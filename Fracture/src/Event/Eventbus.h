#pragma once
#ifndef EVENTBUS_H
#define EVENTBUS_H

#include "Event.h"
#include <unordered_map>
#include <typeinfo>       // operator typeid
#include <typeindex>      // std::type_index
#include <list>
#include "Logging/Logger.h"


namespace Fracture
{
	class HandlerFunctionBase
	{

	public:

		void exec(Event* evnt)
		{
			call(evnt);
		}

	private:
		virtual void call(Event* evnt) = 0;

	};

	template<class T, class EventType>
	class MemberFunctionHandler : public HandlerFunctionBase
	{
	public:
		typedef void (T::* MemberFunction)(EventType*);

		MemberFunctionHandler(T* instance, MemberFunction memberFunction):m_instance(instance), m_memberFunction(memberFunction)
		{

		}
			
		void call(Event* evnt) {
			// Cast event to the correct type and call member function
			(m_instance->*m_memberFunction)(static_cast<EventType*>(evnt));
		}
	private:
		// Pointer to class instance
		T* m_instance;

		// Pointer to member function
		MemberFunction m_memberFunction;
	};


	typedef std::list<HandlerFunctionBase*> HandlerList;
	class Eventbus
	{


	public:
		Eventbus();

		template<class T, class EventType>
		static void Subscribe(T* instance, void(T::* memberFunction)(EventType*))
		{
			HandlerList* handlers = subscribers[typeid(EventType)];

			//First time initialization
			if (handlers == nullptr) {
				handlers = new HandlerList();

				subscribers[typeid(EventType)] = handlers;
			}

			handlers->push_back(new MemberFunctionHandler<T, EventType>(instance, memberFunction));

		}

		template<typename EventType>
		static void Publish(EventType* m_event)
		{		
			HandlerList* handlers = subscribers[typeid(*m_event)];

			if (handlers == nullptr) {
				return;
			}

			for (auto& handler : *handlers) {
				if (handler != nullptr) {
					handler->exec(m_event);
				}
			}

		}


	private:
		static std::unordered_map<std::type_index, HandlerList*> subscribers;


	};


	

}


#endif