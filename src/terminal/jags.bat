setlocal
set JAGS_HOME=c:\Program Files\jags
set PATH=%JAGS_HOME%\bin;%JAGS_HOME%\lib;%PATH%
set LTDL_LIBRARY_PATH=%JAGS_HOME%\modules
jags.bin %1

