#pragma once

namespace Himinn
{
	class GameObject;
	class PlayerComponent;
	enum class ScoreGain;
	
	class Command
	{
	public:
		Command(std::weak_ptr<GameObject> gameObject) : m_GameObject(gameObject) {};
		virtual ~Command() = default;
		virtual void Execute() = 0;
	private:
		std::weak_ptr<GameObject> m_GameObject;
	};

	class JumpCommand final : public Command
	{
	public:
		JumpCommand(std::weak_ptr<GameObject> gameObject);
		~JumpCommand() override = default;
		void Execute() override;
	};

	class ObjectDiesCommand final : public Command
	{
	public:
		ObjectDiesCommand(std::weak_ptr<GameObject> gameObject);
		~ObjectDiesCommand() override = default;
		void Execute() override;
	private:
		std::weak_ptr<PlayerComponent> m_pPlayerComponent = {};
	};

	class GainScoreCommand final : public Command
	{
	public:
		GainScoreCommand(std::weak_ptr<GameObject> gameObject, ScoreGain scoreGain);
		~GainScoreCommand() override = default;
		void Execute() override;
	private:
		std::weak_ptr<PlayerComponent> m_pPlayerComponent = {};
		ScoreGain m_ScoreGain;
	};
}