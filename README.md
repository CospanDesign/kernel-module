# kernel-module: Completion Test Branch


Build both the module and userland application

To run the test go to the root directory and run
```bash
make
```

Install the module
```bash
make install
```

Run the userland application

```bash
./userland/build/mymodule
```

(Copy Pasta Version)
```bash
make
make install
./userland/build/mymodule
```

The application should be blocked waiting for the completion

To run the completion open a second terminal and enter the following:

```bash
sudo bash
echo 1 > /sys/class/modcls/mymodule0/mymodule_test
exit
```

The application should be finished


If you run the application again but instead of calling complete with the sysfs attribute you remove the module with:

```bash
make remove
```

The application and the module is not removed


You can fix this by sending the completion command again






