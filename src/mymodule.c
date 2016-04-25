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
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>   /* copy_to_user */


MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("author");
MODULE_DESCRIPTION("Nysa PCIE Interface");

//-----------------------------------------------------------------------------
// Parameters
//-----------------------------------------------------------------------------

#define MODULE_NAME "mymodule"
#define NUM_MINORS 1

#define SUCCESS 0

static dev_t devno;
struct cdev cdv;

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

  //Request a set of character device numbers
  printk("Registering Driver\n");
  if ((retval = alloc_chrdev_region(&devno, 0, NUM_MINORS, MODULE_NAME)) != 0)
  {
    printk("Failed to create chrdev region");
    goto init_fail;
  }

  //Initialize each of the possible character devices  
  cdev_init(&cdv, &mymodule_fops);
  if ((retval = cdev_add(&cdv, devno, 1)) != 0){
    printk("Error %d while trying to add cdev\n", retval);
    goto probe_destroy_chrdev;
  }

  printk("Driver Initialized!\n");
  return SUCCESS;

//Handle Fail

probe_destroy_chrdev:
  unregister_chrdev_region(devno, 1);
init_fail:
  return retval;
}

static void __exit mymodule_exit(void)
{
  //int major = 0;

  printk("Cleanup Module\n");

  printk("Unregistering Character Driver\n");
  cdev_del(&cdv);
  printk("Give back all the numbers we requested\n");
  unregister_chrdev_region(devno, NUM_MINORS);
  printk("Finished Cleanup Module, Exiting\n");
  return;
}

module_init(mymodule_init);
module_exit(mymodule_exit);


