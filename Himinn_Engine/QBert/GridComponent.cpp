#include "GridComponent.h"

#include <fstream>
#include <iostream>
#include <regex>
#include "Scene.h"
#include "GameObject.h"
#include "GridObserver.h"
#include "NodeObserver.h"
#include "ImageComponent.h"
#include "LiftComponent.h"
#include "NodeComponent.h"
#include "SubjectComponent.h"

GridComponent::GridComponent(const std::weak_ptr<Himinn::GameObject>& owner, std::weak_ptr<Himinn::Scene> scene, const std::string& filePath)
	: Component(owner)
	, m_NodeCanCycle()
	, m_NodeStartLevel()
	, m_NodeLayerAmount()
	, m_NodePlayerOffset()
	, m_NodeTexturePaths()
	, m_NodeLiftLayers()
	, m_NodeLiftTexture()
	, m_pLifts()
	, m_Scene(scene)
	, m_NodeOffset()
	, m_NodesCompleted()
	, m_pNodeObserver(std::make_shared<NodeObserver>())
	, m_pNodes()
{
	ReadGridSettingsFile(filePath);
	GenerateGrid();
}

void GridComponent::FixedUpdate()
{
}

void GridComponent::Update()
{
}

void GridComponent::LateUpdate()
{
}

void GridComponent::Render()
{
}

void GridComponent::OnAddedToObject()
{
	//since we are the nodeobserver, this will always succeed!
	m_pNodeObserver->SetGridComponent(m_Owner.lock()->GetComponent<GridComponent>());
	
	auto subjectComp{ m_Owner.lock()->GetComponent<Himinn::SubjectComponent>() };
	if (subjectComp.expired())
		std::cout << "GridComponent: No SubjectComponent was present, no observations will be made.\n";
	else
		m_pSubjectComponent = subjectComp;
}

void GridComponent::ReadGridSettingsFile(const std::string& filePath)
{
	std::ifstream input{ filePath };
	std::string currentLine{};

	std::regex commentCheck{ "^(.{2}).*" };
	std::regex lineCheck{ "^(.{2})=(.*)$" };
	std::regex trueCheck{ "^(true|TRUE)$" };
	std::regex falseCheck{ "^(false|FALSE)$" };
	std::regex vectorCheck{ "^(-?\\d+);(-?\\d+)$" };
	std::smatch match{};
	
	while (std::getline(input, currentLine))
	{
		// Check if the line is valid and not a comment
		if (!std::regex_match(currentLine, match, commentCheck))
			continue;

		if (match[1] == "//")
			continue;

		// Check the line and set the settings
		if (!std::regex_match(currentLine, match, lineCheck))
			continue;

		std::string check{ match[2] };
		if (match[1] == "NC")
		{
			if (std::regex_match(check, match, trueCheck))
				m_NodeCanCycle = true;
			else if (std::regex_match(check, match, falseCheck))
				m_NodeCanCycle = false;
		}
		else if (match[1] == "SL")
		{
			m_NodeStartLevel = stoi(check);
		}
		else if (match[1] == "LA")
		{
			m_NodeLayerAmount = stoi(check);
		}
		else if (match[1] == "SP")
		{
			if (std::regex_match(check, match, vectorCheck))
				m_Owner.lock()->SetPosition(stof(match[1].str()), stof(match[2].str()));
		}
		else if (match[1] == "PO")
		{
			if (std::regex_match(check, match, vectorCheck))
				m_NodePlayerOffset = Himinn::IVector2{ stoi(match[1].str()) , stoi(match[2].str()) };
		}
		else if (match[1] == "NO")
		{
			if (std::regex_match(check, match, vectorCheck))
				m_NodeOffset = Himinn::IVector2{ stoi(match[1].str()) , stoi(match[2].str()) };
		}
		else if (match[1] == "LL")
		{
			if (std::regex_match(check, match, vectorCheck))
				m_NodeLiftLayers = Himinn::IVector2{ stoi(match[1].str()) , stoi(match[2].str()) };
		}
		else if (match[1] == "TB")
		{
			m_NodeTexturePaths.push_back(check);
		}
		else if (match[1] == "TL")
		{
			m_NodeLiftTexture = check;
		}
	}
}

void GridComponent::UpgradeNode(int layer, int number)
{
	GetNode(layer, number).lock()->GetComponent<NodeComponent>().lock()->IncrementNodeLevel();
}

void GridComponent::UpdateNodeCompletion(bool nodeReady)
{
	if (nodeReady)
	{
		++m_NodesCompleted;
		CheckLevelFinished();
	}
	else
		--m_NodesCompleted;
}

void GridComponent::CheckLevelFinished()
{
	if (m_NodesCompleted == m_pNodes.size()
		&& !m_pSubjectComponent.expired())
		m_pSubjectComponent.lock()->Notify({}, (unsigned)GridObserverEvent::GridCompleted);
}

unsigned GridComponent::GetLayerAmount()
{
	return m_NodeLayerAmount;
}

Himinn::IVector2 GridComponent::GetLeftPeakPosition()
{
	return {(int)m_NodeLayerAmount - 1, 0 };
}

Himinn::IVector2 GridComponent::GetRightPeakPosition()
{
	return { (int)m_NodeLayerAmount - 1, (int)m_NodeLayerAmount - 1 };
}

Himinn::IVector2 GridComponent::GetNodeCharacterPosition(int layer, int number) 
{
	int index{ GetListIndex(layer, number) };
	if (index == -1)
		return { -1, -1 };
	
	auto node = m_pNodes.at(index);
	auto nodePlayerOffset = node->GetComponent<NodeComponent>().lock()->GetPlayerOffset();
	return Himinn::IVector2{ (int)nodePlayerOffset.x + (int)node->GetTransform().GetPosition().x, (int)nodePlayerOffset.y + (int)node->GetTransform().GetPosition().y };
}

std::weak_ptr<Himinn::GameObject> GridComponent::GetNode(int layer, int number) const
{
	int index{ GetListIndex(layer, number) };
	if (index == -1)
		return std::make_shared<Himinn::GameObject>();

	return m_pNodes.at(index);
}

void GridComponent::GenerateGrid()
{
	if (m_Scene.expired())
		return;
	
	auto GridStartPos = m_Owner.lock()->GetTransform().GetPosition();
	
	// Grids
	for (unsigned int i = 0; i < m_NodeLayerAmount; ++i)
	{
		for (unsigned int j = 0; j < (i + 1); ++j)
		{
			auto gameObject{ std::make_shared <Himinn::GameObject>() };
			
			auto subjectComp{ std::make_shared<Himinn::SubjectComponent>(gameObject) };
			subjectComp->AddObserver(m_pNodeObserver);
			gameObject->AddComponent<Himinn::SubjectComponent>(subjectComp);

			auto nodeComp{ std::make_shared<NodeComponent>(gameObject, m_NodeTexturePaths, m_NodePlayerOffset, m_NodeStartLevel, m_NodeCanCycle) };
			gameObject->AddComponent<NodeComponent>(nodeComp);


			Himinn::IVector2 imgDim = gameObject->GetComponent<Himinn::ImageComponent>().lock()->GetTextureDimensions();
			gameObject->SetPosition(GridStartPos.x + ((float)m_NodeOffset.x + imgDim.x) * j - (imgDim.x / 2) * i, GridStartPos.y + ((float)m_NodeOffset.y + imgDim.y) * i);
			
			m_Scene.lock()->Add(gameObject);
			m_pNodes.push_back(gameObject);
		}
	}

	// Lifts
	if (m_NodeLiftLayers.x > (int)m_NodeLayerAmount
		|| m_NodeLiftLayers.x < 1)
		m_NodeLiftLayers.x = m_NodeLayerAmount;

	if (m_NodeLiftLayers.y > (int)m_NodeLayerAmount
		|| m_NodeLiftLayers.y < 1)
		m_NodeLiftLayers.y = m_NodeLayerAmount;

	// Left Lift
	auto lift{ std::make_shared<Himinn::GameObject>() };
	auto liftComp{ std::make_shared<LiftComponent>(lift, m_NodeLiftTexture, Himinn::IVector2{m_NodeLiftLayers.x, -1}, m_NodePlayerOffset) };
	lift->AddComponent<LiftComponent>(liftComp);
	
	Himinn::IVector2 imgDim = lift->GetComponent<Himinn::ImageComponent>().lock()->GetTextureDimensions();
	lift->SetPosition(GridStartPos.x - ((float)m_NodeOffset.x + imgDim.x) - ((imgDim.x / 2) * m_NodeLiftLayers.x), GridStartPos.y + ((float)m_NodeOffset.y + imgDim.y) * m_NodeLiftLayers.x);

	m_Scene.lock()->Add(lift);
	m_pLifts.push_back(lift);
	
	// Left Lift
	lift = std::make_shared<Himinn::GameObject>();
	liftComp = std::make_shared<LiftComponent>(lift, m_NodeLiftTexture, Himinn::IVector2{m_NodeLiftLayers.y, m_NodeLiftLayers.y + 1 }, m_NodePlayerOffset);
	lift->AddComponent<LiftComponent>(liftComp);
	
	imgDim = lift->GetComponent<Himinn::ImageComponent>().lock()->GetTextureDimensions();
	lift->SetPosition(GridStartPos.x + ((float)m_NodeOffset.x + imgDim.x) * (m_NodeLiftLayers.y + 1) - ((imgDim.x / 2) * m_NodeLiftLayers.y), GridStartPos.y + ((float)m_NodeOffset.y + imgDim.y) * m_NodeLiftLayers.y);

	m_Scene.lock()->Add(lift);
	m_pLifts.push_back(lift);
}

int GridComponent::GetListIndex(int layer, int number) const
{
	if (number >= layer + 1
		|| number < 0)
		return -1;
	
	int index{};
	for (int i = 1; i <= layer; ++i)
		index += i;

	if (index >= (int)m_pNodes.size())
		return -1;
	
	return (index + number);
}

bool GridComponent::CheckForLift(int layer, int number)
{
	for (auto& lift : m_pLifts)
	{
		auto liftComp = lift->GetComponent<LiftComponent>();
		if (liftComp.expired()
			|| liftComp.lock()->GetGridPosition() != Himinn::IVector2{layer, number})
			continue;

		liftComp.lock()->Activate();
		m_pLifts.erase(std::remove(m_pLifts.begin(), m_pLifts.end(), lift));
		return true;
	}

	return false;
}
