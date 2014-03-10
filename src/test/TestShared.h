#pragma once
#include <common/Compiler.h>
#include <gtest/gtest.h>
#include <AI.h>
#include <AIRegistry.h>
#include <pathfinding/IPathfinder.h>

class TestSuite: public ::testing::Test {
protected:
	ai::AIRegistry _registry;
	ai::NOPPathfinder _pathfinder;

	virtual void SetUp() override;
	virtual void TearDown() override;
};
