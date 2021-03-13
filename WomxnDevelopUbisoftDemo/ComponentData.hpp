#pragma once

#include <unordered_map>

class IComponentData
{
};

template <typename T>
class ComponentData : public IComponentData
{
public:
	void AddComponent(Entity entity, T component)
	{
		componentData.insert({ entity, component });
	}

	T GetComponent(Entity entity)
	{
		auto it = componentData.find(entity);

		//if (it == componentData.end()) {
			//return NULL;
		//}

		return it->second;
	}

private:
	std::unordered_map<Entity, T> componentData;
};