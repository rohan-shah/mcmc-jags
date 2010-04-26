SUBDIRS = runtime

EXTRA_DIST=jags.nsi jags64.nsi AdvUninstLog.nsh MultiUser64.nsh README.WIN

clean-local:
	rm -rf inst 

win32-installer: 
	makensis -nocd -DVERSION=${PACKAGE_VERSION} -DJAGSINC=$(srcdir) \
	-DLICENSE=$(top_srcdir)/COPYING $(srcdir)/jags.nsi

win64-installer:
	makensis -nocd -DVERSION=${PACKAGE_VERSION} -DJAGSINC=$(srcdir) \
	-DLICENSE=$(top_srcdir)/COPYING $(srcdir)/jags64.nsi
