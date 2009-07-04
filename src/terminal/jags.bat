setlocal
set PATH=@JAGS_HOME@\bin;%PATH%
set LTDL_LIBRARY_PATH=@JAGS_HOME@\modules-@VERSION@
cls
jags-terminal %1

