lib_LTLIBRARIES = libsimpleai.la

libsimpleai_la_SOURCES = \
	SimpleAI.cpp

library_includedir = $(includedir)/simpleai
nobase_library_include_HEADERS = \
	aggro/AggroMgr.h \
	aggro/Entry.h \
	AI.h \
	AIFactories.h \
	AIRegistry.h \
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
	common/ThreadPool.h \
	common/ThreadScheduler.h \
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
	filter/SelectGroupLeader.h \
	filter/SelectGroupMembers.h \
	filter/SelectHighestAggro.h \
	filter/SelectZone.h \
	IAIFactory.h \
	ICharacter.h \
	group/GroupId.h \
	group/GroupMgr.h \
	movement/SelectionSeek.h \
	movement/GroupFlee.h \
	movement/GroupSeek.h \
	movement/Steering.h \
	movement/TargetFlee.h \
	movement/TargetSeek.h \
	movement/Wander.h \
	movement/WeightedSteering.h \
	server/AIAddNodeMessage.h \
	server/AIChangeMessage.h \
	server/AICharacterDetailsMessage.h \
	server/AICharacterStaticMessage.h \
	server/AIDeleteNodeMessage.h \
	server/AINamesMessage.h \
	server/AIPauseMessage.h \
	server/AISelectMessage.h \
	server/AIStateMessage.h \
	server/AIStepMessage.h \
	server/AIStubTypes.h \
	server/AIUpdateNodeMessage.h \
	server/AddNodeHandler.h \
	server/ChangeHandler.h \
	server/DeleteNodeHandler.h \
	server/IProtocolHandler.h \
	server/IProtocolMessage.h \
	server/Network.h \
	server/NetworkImpl.h \
	server/PauseHandler.h \
	server/ProtocolHandlerRegistry.h \
	server/ProtocolMessageFactory.h \
	server/ResetHandler.h \
	server/SelectHandler.h \
	server/Server.h \
	server/ServerImpl.h \
	server/StepHandler.h \
	server/UpdateNodeHandler.h \
	zone/Zone.h \
	SimpleAI.h \
	tree/Fail.h \
	tree/Limit.h \
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
	tree/TreeNodeImpl.h \
	tree/TreeNodeParser.h

if AI_ENABLE_LUA
nobase_library_include_HEADERS += \
	tree/loaders/lua/LUA.h \
	tree/loaders/lua/LUATreeLoader.h
endif

if AI_ENABLE_XML
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

AM_CPPFLAGS += -I$(top_builddir)/src/libs

libsimpleai_la_CXXFLAGS = @PTHREAD_CFLAGS@ $(AM_CXXFLAGS) -Wall -Wextra -Wabi -Wcast-qual -Wconversion -Wfloat-equal -Wmissing-declarations -Wmissing-format-attribute -Wno-long-long -Woverloaded-virtual -Wpacked -Wredundant-decls -Wshadow -Wsign-promo -Wno-sign-compare

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
