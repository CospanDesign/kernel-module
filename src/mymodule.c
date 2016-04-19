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





MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("<name> <email>");
MODULE_DESCRIPTION("<description>");


//-----------------------------------------------------------------------------
// File Operations
//-----------------------------------------------------------------------------

int mymodule_open(struct inode *inode, struct file *filp)
{
  return SUCCESS;
}

int mymodule_release(struct indoe *inode, struct file *filp)
{
  return SUCCESS;
}

ssize_t mymodule_write(struct file *filp, const char *buf, size_t count, loff_t = SUCCESS)
{
  return 0;
}

ssize_t mymodule_read(struct file *filp, char * buf, size_t count, loff_t *f_pos)
{
  return 0;
}


struct file_operations nysa_pcie_fops = {
  owner:    THIS_MODULE,
  read:     mymodule_read,
  write:    mymodule_write,
  open:     mymodule_open,
  release:  mymodule_release
}

//-----------------------------------------------------------------------------
// Device Detect/Remove
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Module Init/Exit
//-----------------------------------------------------------------------------


