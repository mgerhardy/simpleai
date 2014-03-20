lib_LTLIBRARIES = libsimpleai.la

libsimpleai_la_SOURCES = \
	aggro/AggroMgr.cpp \
	conditions/And.cpp \
	conditions/False.cpp \
	conditions/HasEnemies.cpp \
	conditions/ICondition.cpp \
	conditions/Not.cpp \
	conditions/Or.cpp \
	conditions/True.cpp \
	conditions/ConditionParser.cpp \
	tree/Invert.cpp \
	tree/Sequence.cpp \
	tree/PrioritySelector.cpp \
	tree/Parallel.cpp \
	tree/Idle.cpp \
	tree/ITimedNode.cpp \
	tree/TreeNode.cpp \
	tree/TreeNodeParser.cpp \
	tree/ITask.cpp \
	tree/loaders/lua/LUA.cpp \
	tree/loaders/lua/LUANode.cpp \
	tree/loaders/lua/LUATree.cpp \
	tree/loaders/lua/LUATreeLoader.cpp \
	server/IProtocolMessage.cpp \
	server/ProtocolMessageFactory.cpp \
	server/Server.cpp \
	server/Network.cpp \
	AIRegistry.cpp \
	AI.cpp
	
library_includedir = $(includedir)/simpleai
nobase_library_include_HEADERS = \
	AI.h \
	ICharacter.h \
	IAIFactory.h \
	AIRegistry.h \
	aggro/Entry.h \
	aggro/AggroMgr.h \
	tree/Invert.h \
	tree/Parallel.h \
	tree/Idle.h \
	tree/ITimedNode.h \
	tree/TreeNode.h \
	tree/ITask.h \
	tree/TreeNodeParser.h \
	tree/PrioritySelector.h \
	tree/Sequence.h \
	tree/loaders/ITreeLoader.h \
	tree/loaders/lua/LUATreeLoader.h \
	common/Enum.h \
	common/IFactoryRegistry.h \
	common/IPrintable.h \
	common/IParser.h \
	common/Compiler.h \
	common/NonCopyable.h \
	common/Pointers.h \
	common/AIPosition.h \
	common/AIException.h \
	pathfinding/IPathfinder.h \
	pathfinding/IMap.h \
	server/AIStateMessage.h \
	server/AISelectMessage.h \
	server/AICharacterDetailsMessage.h \
	server/AIStubTypes.h \
	server/IProtocolHandler.h \
	server/IProtocolMessage.h \
	server/SelectHandler.h \
	server/Network.h \
	server/ProtocolHandlerRegistry.h \
	server/ProtocolMessageFactory.h \
	server/Server.h \
	conditions/ConditionParser.h \
	conditions/Not.h \
	conditions/False.h \
	conditions/HasEnemies.h \
	conditions/True.h \
	conditions/Or.h \
	conditions/ICondition.h \
	conditions/And.h

EXTRA_DIST =

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include -I$(top_builddir)/src/libs/lua

libsimpleai_la_CFLAGS = -Wall -DLUA_USE_LONGJMP

libsimpleai_la_LDFLAGS = -no-undefined -release @VERSION@ -lm

libsimpleai_la_LIBADD = \
	@MATH_LIBS@ \
	../libs/libexternal.la
