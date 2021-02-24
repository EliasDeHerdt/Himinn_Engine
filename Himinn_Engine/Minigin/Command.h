#pragma once
#include "GameObject.h"

using namespace Himinn;
class Command
{
public:
	virtual ~Command() = default;
	virtual void Execute(const GameObject&) = 0;
};

class ColorCommand final : public Command
{
public:
	virtual ~ColorCommand() override = default;
	virtual void Execute(const GameObject&) override {};
};

class FireCommand final : public Command
{
public:
	virtual ~FireCommand() override = default;
	virtual void Execute(const GameObject&) override;
};

class DuckCommand final : public Command
{
public:
	virtual ~DuckCommand() override = default;
	virtual void Execute(const GameObject&) override;
};

class JumpCommand final : public Command
{
public:
	virtual ~JumpCommand() override = default;
	virtual void Execute(const GameObject&) override;
};

class FartCommand final : public Command
{
public:
	virtual ~FartCommand() override = default;
	virtual void Execute(const GameObject&) override;
};