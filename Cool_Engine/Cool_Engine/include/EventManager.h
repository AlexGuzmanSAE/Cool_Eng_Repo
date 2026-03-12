#pragma once

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <functional>
#include <algorithm>

class EventManager {

public:
	using Callback = std::function<void(const void*)>; // Accion que se ejecuta al emitir el evento
	using Listener = std::pair<void*, Callback>; // Objeto y accion asociada

	static EventManager& instance()
	{
		static EventManager instance;
		return instance;
	}

	template<typename Event, typename Object>
	void suscribe(Object* object, void(Object::* method)(const Event&))
	{
		auto eventType = std::type_index(typeid(Event)); // id del tipo del evento
		Callback callBack = [object, method](const void* evt) // apuntador a funcion miebro
			{
				const Event* event = static_cast<const Event*>(evt); // conversion de void*
				(object->*method)(*event);// ejecucion del metodo del objecto
			};
		listeners[eventType].push_back({ object, callBack }); //guardar listener
	}


	template<typename Event>
	void unsuscribe(void* object)
	{
		auto eventType = std::type_index(typeid(Event)); //tipo del evento
		auto it = listeners.find(eventType);				 // Buscar lista de oyentes
		if (it == listeners.end()) return;				 // Si no hay listeners, salir

		auto& list = it->second;						 // Lista de listeners
		list.erase(std::remove_if(list.begin(), list.end(), [object](const Listener& lst)
			{return list.first == object; }), list.end());  // remueve y borra un listener si el obj coincide
	}

	template<typename Event>
	void emit(const Event& event)
	{
		auto eventType = std::type_index(typeid(Event));
		auto it = listeners.find(eventType);
		if (it == listeners.end()) return;

		for (const Listener& lst : it->second)
		{
			lst.second(&event);
		}
	}

private:

	std::unordered_map <std::type_index, std::vector<Listener>> listeners;

};