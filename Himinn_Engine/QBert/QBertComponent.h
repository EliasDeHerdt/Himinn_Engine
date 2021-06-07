#pragma once
#include "ControllableComponent.h"
#include <memory>
#include <string>


namespace Himinn {
	class SubjectComponent;
	class ImageComponent;
	class GameObject;
}

enum class ScoreGain
{
	ColorChange = 25,
	DiscRemaining = 50,
	Catch = 300,
	DefeatCoily = 500
};


class QBertComponent : public ControllableComponent
{
public:
	QBertComponent(const std::weak_ptr<Himinn::GameObject>& owner, int lives, std::string TexturePath);
	virtual ~QBertComponent() override = default;
	QBertComponent(const QBertComponent& other) = delete;
	QBertComponent& operator=(const QBertComponent& other) = delete;
	QBertComponent(QBertComponent&& other) = delete;
	QBertComponent& operator=(QBertComponent&& other) = delete;

	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void OnAddedToObject() override;

	void OnDeath() override;
	void OnScore(int amount) override;

	int GetLives() const;
	int GetScore() const;

	void LoseLife();
	void SetLives(int lives);
	void SetScore(int score);
	void GainScore(int score);

private:
	int m_Lives;
	int m_Score;
	std::string m_TexturePath;
	
	std::weak_ptr<Himinn::ImageComponent> m_pImageComponent;
	std::weak_ptr<Himinn::SubjectComponent> m_pSubjectComponent;
};
