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
	server/PauseHandler.cpp \
	server/SelectHandler.cpp \
	server/Server.cpp \
	server/Network.cpp \
	AIRegistry.cpp \
	AI.cpp
	
library_includedir = $(includedir)/simpleai
nobase_library_include_HEADERS = \
	aggro/AggroMgr.h \
	aggro/Entry.h \
	AI.h \
	AIRegistry.h \
	common/AIException.h \
	common/AIPosition.h \
	common/Compiler.h \
	common/Enum.h \
	common/IFactoryRegistry.h \
	common/IParser.h \
	common/IPrintable.h \
	common/NonCopyable.h \
	common/Pointers.h \
	conditions/And.h \
	conditions/ConditionParser.h \
	conditions/False.h \
	conditions/HasEnemies.h \
	conditions/ICondition.h \
	conditions/Not.h \
	conditions/Or.h \
	conditions/True.h \
	IAIFactory.h \
	ICharacter.h \
	pathfinding/IMap.h \
	pathfinding/IPathfinder.h \
	server/AICharacterDetailsMessage.h \
	server/AIPauseMessage.h \
	server/AISelectMessage.h \
	server/AIStateMessage.h \
	server/AIStubTypes.h \
	server/IProtocolHandler.h \
	server/IProtocolMessage.h \
	server/Network.h \
	server/PauseHandler.h \
	server/ProtocolHandlerRegistry.h \
	server/ProtocolMessageFactory.h \
	server/SelectHandler.h \
	server/Server.h \
	SimpleAI.h \
	tree/Idle.h \
	tree/Invert.h \
	tree/ITask.h \
	tree/ITimedNode.h \
	tree/loaders/ITreeLoader.h \
	tree/loaders/lua/LUACondition.h \
	tree/loaders/lua/LUAFunctions.h \
	tree/loaders/lua/LUA.h \
	tree/loaders/lua/LUANode.h \
	tree/loaders/lua/LUATree.h \
	tree/loaders/lua/LUATreeLoader.h \
	tree/Parallel.h \
	tree/PrioritySelector.h \
	tree/Selector.h \
	tree/Sequence.h \
	tree/TreeNode.h \
	tree/TreeNodeParser.h

EXTRA_DIST =

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include -I$(top_builddir)/src/libs/lua

libsimpleai_la_CFLAGS = -Wall -DLUA_USE_LONGJMP

libsimpleai_la_LDFLAGS = -no-undefined -release @VERSION@ -lm

libsimpleai_la_LIBADD = \
	@MATH_LIBS@ \
	../libs/libexternal.la
