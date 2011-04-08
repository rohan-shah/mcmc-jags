setlocal
set PATH=@JAGS_HOME@\bin\;%PATH%
set LTDL_LIBRARY_PATH=@JAGS_HOME@\modules
cls
jags-terminal %1

