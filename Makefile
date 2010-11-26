
PACKAGE=net.autch.android.fluidballsgl
CLASSNAME=FluidBallsRenderer

BINDIR=bin
OUTDIR=jni
CLASSPATH=$(BINDIR)

PKG_PATH=$(shell echo $(PACKAGE) | tr . /)
PKG_JNI=$(shell echo $(PACKAGE) | tr . _)
CLS_JNI=$(shell echo $(CLASSNAME) | sed "s/_/_1/g")

$(OUTDIR)/$(PKG_JNI)_$(CLS_JNI).h: $(BINDIR)/$(PKG_PATH)/$(CLASSNAME).class
	javah -classpath $(CLASSPATH) -d $(OUTDIR) $(PACKAGE).$(CLASSNAME)

