/*
 * mymodule
 *
 * Copyright (c) 2016 <your company name>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/* This code has is influenced heavily from the following sources:
 *  Adrian Byszuk
 *  URL: https://github.com/abyszuk/fpga_pcie_driver
 *
 *  Eugene ?? http://stackoverflow.com/users/689077/eugene
 *  URL: https://github.com/euspectre/kedr/blob/master/sources/examples/sample_target/cfake.c
 *  URL: https://github.com/euspectre/kedr/blob/master/sources/examples/sample_target/cfake.h
 */

#include <linux/types.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>   /* copy_to_user */
#include <linux/completion.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("author");
MODULE_DESCRIPTION("module demo");

//-----------------------------------------------------------------------------
// Parameters
//-----------------------------------------------------------------------------

#define MODULE_NAME "mymodule"
#define CLASS_NAME "modcls"
#define NUM_MINORS 1

#define SUCCESS 0

static dev_t devno;

typedef struct {
  struct cdev cdv;
  struct class *cls;
  struct device *dev;
  struct completion complete;
} mymodule_t;

mymodule_t mymod;

//-----------------------------------------------------------------------------
// Sysfs Interface
//-----------------------------------------------------------------------------
static ssize_t mymodule_test_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
  int retval = 0;
  int value = 0;
  if (sscanf(buf, "%d", &value) == 1)
  {
    retval = strlen(buf);
  }
  if (value)
  {
    printk("Value is: %d\n", value);
    if (!completion_done(&mymod.complete))
    {
      complete(&mymod.complete);
    }
    printk("Sent Completion\n");
  }

  return retval;
}
static ssize_t mymodule_test_show(struct device *dev, struct device_attribute *attr, char *buf)
{
  return snprintf(buf, PAGE_SIZE, "hello!\n");
}

static DEVICE_ATTR_RW(mymodule_test);

static struct attribute * mymodule_sysfs_attrs [] =
{
 &dev_attr_mymodule_test.attr,
 NULL,
};

ATTRIBUTE_GROUPS(mymodule_sysfs);


//-----------------------------------------------------------------------------
// File Operations
//-----------------------------------------------------------------------------

int mymodule_open(struct inode *inode, struct file *filp)
{
  printk("Opened!\n");
  return SUCCESS;
}

int mymodule_release(struct inode *inode, struct file *filp)
{
  printk("Closed!\n");
  return SUCCESS;
}

ssize_t mymodule_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
  printk("Write!\n");
  return 0;
}

ssize_t mymodule_read(struct file *filp, char * buf, size_t count, loff_t *f_pos)
{
  printk("Read!\n");
  if (completion_done(&mymod.complete))
  {
    reinit_completion(&mymod.complete);
  }

  printk("Wait for Completion\n");
  wait_for_completion_interruptible(&mymod.complete);
  printk("After Completion\n");
  return 0;
}

struct file_operations mymodule_fops = {
  owner:    THIS_MODULE,
  read:     mymodule_read,
  write:    mymodule_write,
  open:     mymodule_open,
  release:  mymodule_release
};

//-----------------------------------------------------------------------------
// Module Init/Exit
//-----------------------------------------------------------------------------

static int __init mymodule_init(void)
{
  int retval = SUCCESS;
  int index = 0;

  //Request a set of character device numbers
  printk("Registering Driver\n");
  if ((retval = alloc_chrdev_region(&devno, 0, NUM_MINORS, MODULE_NAME)) != 0)
  {
    printk("Failed to create chrdev region\n");
    goto init_fail;
  }
  mymod.cls = class_create(THIS_MODULE, CLASS_NAME);
  if (IS_ERR(mymod.cls))
  {
    retval = PTR_ERR(mymod.cls);
    printk("Failed to create module class\n");
    goto probe_class_fail;
  }

  mymod.dev = device_create_with_groups(mymod.cls,
                                        NULL,
                                        MKDEV(MAJOR(devno), index),
                                        NULL,
                                        mymodule_sysfs_groups,
                                        MODULE_NAME "%d", index);
  if (IS_ERR(mymod.dev))
  {
    retval = PTR_ERR(mymod.dev);
    printk("Failed to create module class device\n");
    goto probe_device_class_fail;
  }

  //Initialize each of the possible character devices
  cdev_init(&mymod.cdv, &mymodule_fops);
  if ((retval = cdev_add(&mymod.cdv, devno, 1)) != 0){
    printk("Error %d while trying to add cdev\n", retval);
    goto probe_cdev_init_fail;
  }
  init_completion(&mymod.complete);

  printk("Driver Initialized!\n");
  return SUCCESS;

//Handle Fail
probe_cdev_init_fail:
  device_destroy(mymod.cls, MKDEV(MAJOR(devno), 0));
probe_device_class_fail:
  class_destroy(mymod.cls);
probe_class_fail:
  unregister_chrdev_region(devno, 1);
init_fail:
  return retval;
}

static void __exit mymodule_exit(void)
{
  printk("Cleanup Module\n");

  printk("Check if we need to complete anything\n");
  if (!completion_done(&mymod.complete))
  {
    printk("Send a completion!\n");
    complete(&mymod.complete);
  }

  printk("Unregistering Character Driver\n");
  cdev_del(&mymod.cdv);
  printk("Give back all the numbers we requested\n");
  unregister_chrdev_region(devno, NUM_MINORS);
  printk("Remove the class driver\n");
  device_destroy(mymod.cls, MKDEV(MAJOR(devno), 0));
  printk("Release the class\n");
  class_unregister(mymod.cls);
  class_destroy(mymod.cls);
  printk("Finished Cleanup Module, Exiting\n");
  return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);


