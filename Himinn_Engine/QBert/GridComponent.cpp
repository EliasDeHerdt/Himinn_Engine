#include "GridComponent.h"

#include <fstream>
#include <regex>
#include "Scene.h"
#include "GameObject.h"
#include "ImageComponent.h"
#include "NodeComponent.h"

GridComponent::GridComponent(const std::weak_ptr<Himinn::GameObject>& owner, Himinn::Scene& scene, const std::string& filePath)
	: Component(owner)
	, m_NodeCanCycle()
	, m_NodeLayers()
	, m_NodeStartLevel()
	, m_NodePlayerOffset()
	, m_NodeTexturePaths()
	, m_Scene(scene)
	, m_NodeOffset()
	, m_pGameObjects()
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
			m_NodeLayers = stoi(check);
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
		else if (match[1] == "TP")
		{
			m_NodeTexturePaths.push_back(check);
		}
	}
}

void GridComponent::GenerateGrid()
{
	for (unsigned int i = 0; i < m_NodeLayers; ++i)
	{
		for (unsigned int j = 0; j < (i + 1); ++j)
		{
			auto gameObject{ std::make_shared <Himinn::GameObject>() };
			auto gridComp{ std::make_shared<NodeComponent>(gameObject, m_NodeTexturePaths, m_NodePlayerOffset, m_NodeStartLevel, m_NodeCanCycle) };
			gameObject->AddComponent<NodeComponent>(gridComp);

			int index{};
			// TopLeft
			if ((index = GetListIndex(i - 1, j - 1)) != -1)
			{
				auto topLeft = m_pGameObjects.at(index)->GetComponent<NodeComponent>();
				gridComp->SetNeighbor(topLeft, Himinn::QBertDirection::TopLeft);
				topLeft.lock()->SetNeighbor(gridComp, Himinn::QBertDirection::BottomRight);
			}
			
			// TopRight
			if ((index = GetListIndex(i - 1, j)) != -1)
			{
				auto topRight = m_pGameObjects.at(index)->GetComponent<NodeComponent>();
				gridComp->SetNeighbor(topRight, Himinn::QBertDirection::TopRight);
				topRight.lock()->SetNeighbor(gridComp, Himinn::QBertDirection::BottomLeft);
			}

			Himinn::IVector2 imgDim = gameObject->GetComponent<Himinn::ImageComponent>().lock()->GetTextureDimensions();
			gameObject->SetPosition(m_Owner.lock()->GetTransform().GetPosition().x + ((float)m_NodeOffset.x + imgDim.x) * j - (imgDim.x / 2) * i, m_Owner.lock()->GetTransform().GetPosition().y + ((float)m_NodeOffset.y + imgDim.y) * i);
			
			m_Scene.Add(gameObject);
			m_pGameObjects.push_back(gameObject);
		}
	}
}

int GridComponent::GetListIndex(int layer, int number)
{
	if (number >= layer + 1
		&& number < 0)
		return -1;
	
	int index{};
	for (int i = 1; i <= layer; ++i)
		index += i;

	if (index >= m_pGameObjects.size())
		return -1;
	
	return (index + number);
}
