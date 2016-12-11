# cosmo_sys

cosmo_sys is an application running platform. Complecated application can usually be logically splited into several
submodules and each of these submodule focuses only a relatively independent function. submodules communicates with 
each other to fullfil the overall bigger function.

cosmo_sys allows this logical spliting of functionalities into reality by allowing applications to be built into 
several seperated building blocks. And the cosmo_sys provides messaging service to these building blocks and 
triggers/schedules each building block to run.

By this system, application developpers need only focus on the logic/spliting logic of applicaiton itself, cosmo_sys 
will handle the remaining task for it.
