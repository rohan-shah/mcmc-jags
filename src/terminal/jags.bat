setlocal
set JAGS_HOME=@JAGS_HOME@
set PATH=%JAGS_HOME%\bin;%JAGS_HOME%\lib;%PATH%
set LTDL_LIBRARY_PATH=%JAGS_HOME%\modules
jags-terminal %1

