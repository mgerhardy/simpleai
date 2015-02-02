lib_LTLIBRARIES = libsimpleai.la

libsimpleai_la_SOURCES = \
	aggro/AggroMgr.cpp \
	AI.cpp \
	AIRegistry.cpp \
	common/Vector3f.cpp \
	conditions/And.cpp \
	conditions/ConditionParser.cpp \
	conditions/False.cpp \
	conditions/Filter.cpp \
	conditions/HasEnemies.cpp \
	conditions/ICondition.cpp \
	conditions/IsCloseToGroup.cpp \
	conditions/IsGroupLeader.cpp \
	conditions/IsInGroup.cpp \
	conditions/Not.cpp \
	conditions/Or.cpp \
	conditions/True.cpp \
	filter/SelectEmpty.cpp \
	filter/SelectGroupMembers.cpp \
	filter/SelectHighestAggro.cpp \
	movement/Steering.cpp \
	group/GroupMgr.cpp \
	server/ChangeHandler.cpp \
	server/Network.cpp \
	server/PauseHandler.cpp \
	server/ProtocolMessageFactory.cpp \
	server/ResetHandler.cpp \
	server/SelectHandler.cpp \
	server/Server.cpp \
	server/StepHandler.cpp \
	zone/Zone.cpp \
	tree/Idle.cpp \
	tree/Invert.cpp \
	tree/ITask.cpp \
	tree/ITimedNode.cpp \
	tree/Parallel.cpp \
	tree/PrioritySelector.cpp \
	tree/ProbabilitySelector.cpp \
	tree/RandomSelector.cpp \
	tree/Sequence.cpp \
	tree/Steer.cpp \
	tree/Succeed.cpp \
	tree/TreeNode.cpp \
	tree/TreeNodeParser.cpp

library_includedir = $(includedir)/simpleai
nobase_library_include_HEADERS = \
	aggro/AggroMgr.h \
	aggro/Entry.h \
	AI.h \
	AIRegistry.h \
	common/Vector3f.h \
	common/IFactoryRegistry.h \
	common/IParser.h \
	common/IPrintable.h \
	common/Math.h \
	common/MemoryAllocator.h \
	common/MoveVector.h \
	common/NonCopyable.h \
	common/Random.h \
	common/String.h \
	common/Thread.h \
	common/Types.h \
	conditions/And.h \
	conditions/ConditionParser.h \
	conditions/False.h \
	conditions/Filter.h \
	conditions/HasEnemies.h \
	conditions/ICondition.h \
	conditions/IsCloseToGroup.h \
	conditions/IsGroupLeader.h \
	conditions/IsInGroup.h \
	conditions/Not.h \
	conditions/Or.h \
	conditions/True.h \
	filter/IFilter.h \
	filter/SelectEmpty.h \
	filter/SelectGroupMembers.h \
	filter/SelectHighestAggro.h \
	IAIFactory.h \
	ICharacter.h \
	group/GroupMgr.h \
	movement/Steering.h \
	server/AIChangeMessage.h \
	server/AICharacterDetailsMessage.h \
	server/AICharacterStaticMessage.h \
	server/AINamesMessage.h \
	server/AIPauseMessage.h \
	server/AISelectMessage.h \
	server/AIStateMessage.h \
	server/AIStepMessage.h \
	server/AIStubTypes.h \
	server/ChangeHandler.h \
	server/IProtocolHandler.h \
	server/IProtocolMessage.h \
	server/Network.h \
	server/PauseHandler.h \
	server/ProtocolHandlerRegistry.h \
	server/ProtocolMessageFactory.h \
	server/ResetHandler.h \
	server/SelectHandler.h \
	server/Server.h \
	server/StepHandler.h \
	zone/Zone.h \
	SimpleAI.h \
	tree/Idle.h \
	tree/Invert.h \
	tree/ITask.h \
	tree/ITimedNode.h \
	tree/loaders/ITreeLoader.h \
	tree/Parallel.h \
	tree/Print.h \
	tree/PrioritySelector.h \
	tree/ProbabilitySelector.h \
	tree/RandomSelector.h \
	tree/Selector.h \
	tree/Sequence.h \
	tree/Steer.h \
	tree/Succeed.h \
	tree/TreeNode.h \
	tree/TreeNodeParser.h

if AI_ENABLE_LUA
libsimpleai_la_SOURCES += \
	tree/loaders/lua/LUA.cpp \
	tree/loaders/lua/LUANode.cpp \
	tree/loaders/lua/LUATree.cpp \
	tree/loaders/lua/LUATreeLoader.cpp

nobase_library_include_HEADERS += \
	tree/loaders/lua/LUACondition.h \
	tree/loaders/lua/LUAFunctions.h \
	tree/loaders/lua/LUA.h \
	tree/loaders/lua/LUANode.h \
	tree/loaders/lua/LUATree.h \
	tree/loaders/lua/LUATreeLoader.h
endif

if AI_ENABLE_XML
libsimpleai_la_SOURCES += \
	tree/loaders/xml/XMLTreeLoader.cpp

nobase_library_include_HEADERS += \
	tree/loaders/xml/XMLTreeLoader.h
endif

EXTRA_DIST =

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_builddir)/include

if AI_ENABLE_LUA
AM_CPPFLAGS += -I$(top_builddir)/src/libs/lua
endif

if AI_ENABLE_XML
AM_CPPFLAGS += -I$(top_builddir)/src/libs/tinyxml2
endif

libsimpleai_la_CXXFLAGS = @PTHREAD_CFLAGS@ $(AM_CXXFLAGS) -Wall -pedantic -Wextra -Wabi -Wcast-qual -Wconversion -Wfloat-equal -Wmissing-declarations -Wmissing-format-attribute -Wno-long-long -Woverloaded-virtual -Wpacked -Wredundant-decls -Wshadow -Wsign-promo -Wno-sign-compare

libsimpleai_la_LDFLAGS = -no-undefined -release @VERSION@

libsimpleai_la_LIBADD = @MATH_LIBS@ @PTHREAD_LIBS@

if AI_ENABLE_LUA
libsimpleai_la_LIBADD += ../libs/lua/liblua.la
if !AI_EXCEPTIONS
libsimpleai_la_CXXFLAGS += -DLUA_USE_LONGJMP
endif
endif

if AI_ENABLE_XML
libsimpleai_la_LIBADD += ../libs/tinyxml2/libtinyxml2.la
endif
