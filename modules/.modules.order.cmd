cmd_/media/sf_so/KernelModule/modules/modules.order := {   echo /media/sf_so/KernelModule/modules/keylogger_mod.ko; :; } | awk '!x[$$0]++' - > /media/sf_so/KernelModule/modules/modules.order
