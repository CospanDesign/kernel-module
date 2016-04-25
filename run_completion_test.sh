#! /bin/bash


printf "Build and install module\n"
make
printf "Install Module"
make install
printf "\n\n"
printf "Completion Test\n"
printf "\tstart the userland read app...\n"
printf "\tThe app should be blocked\n"
printf "\tTo continue use the following commands\n"
printf "\n"
printf "\tsudo bash\n"
printf "\techo 1 > /sys/class/modcls/mymodule0/mymodule_test\n"
printf "\texit\n"

./userland/build/mymodule

printf "\tUser application is no longer blocked\n"
printf "\tModule can be removed with:\n"
printf "\t\tmake remove\n"
printf "\n"
printf "Re-run this test but instead of sending completion signal remove the module with \'make remove\'\n"


