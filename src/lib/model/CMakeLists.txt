add_library(model OBJECT SymTab.cc NodeArray.cc Model.cc Monitor.cc BUGSModel.cc MonitorFactory.cc MonitorControl.cc MonitorInfo.cc CODA.cc NodeArraySubset.cc)
if(NOT WIN32)
	target_compile_options(model PRIVATE -fPIC)
endif()
