// Fill out your copyright notice in the Description page of Project Settings.

#include "MapStruct.h"
#include "Engine.h"

using namespace MapStruct;

void MapStruct::ItemSpawnQueueInit(GItemSpawnQueue * _queue)
{
	_queue->Front = nullptr;
	_queue->Rear = nullptr;
}

bool MapStruct::ItemSpawnQueueIsEmpty(GItemSpawnQueue * _queue)
{
	if (_queue->Front == nullptr)
		return true;

	else
		return false;
}

void MapStruct::ItemSpawnEnQueue(GItemSpawnQueue * _queue, int _data)
{
	GItemSpawnNode * newNode = new GItemSpawnNode;

	newNode->next = nullptr;
	newNode->Data = _data;
	
	if (ItemSpawnQueueIsEmpty(_queue))
	{
		_queue->Front = newNode;
		_queue->Rear = newNode;
	}
	else
	{
		_queue->Rear->next = newNode;
		_queue->Rear = newNode;
	}
}

int MapStruct::ItemSpawnDeQueue(GItemSpawnQueue * _queue)
{
	GItemSpawnNode * DelNode;
	int RetData = 0;

	if (ItemSpawnQueueIsEmpty(_queue))
	{
		return RetData;
	}

	DelNode = _queue->Front;
	RetData = DelNode->Data;
	_queue->Front = _queue->Front->next;

	delete(DelNode);

	return RetData;
}

void MapStruct::ItemSpawnDeleteQueue(GItemSpawnQueue * _queue, int _data)
{
	GItemSpawnNode * Node = nullptr;
	Node = _queue->Front;
	GItemSpawnNode * RearNode = nullptr;

	if (!ItemSpawnQueueIsEmpty(_queue))
	{
		for (;;)
		{
			if (Node->Data == _data)
			{
				if (RearNode == nullptr)
				{
					ItemSpawnDeQueue(_queue);
					return;
				}

				RearNode->next = Node->next;
				delete(Node);
				return;
			}

			RearNode = Node;
			Node = RearNode->next;

			if (Node == nullptr)
				return;
		}
	}
}

void MapStruct::WeatherListInit(GWeatherList * _list)
{
	_list->Head = nullptr;
	_list->Tail = nullptr;
	_list->Cur = nullptr;

	_list->NumOfData = 0;
}

GWeatherNode * MapStruct::WeatherListAdd(GWeatherList * _list, GWeatherData _data)
{
	_list->NumOfData++;
	GWeatherNode * newNode = new GWeatherNode;

	newNode->Next = nullptr;
	newNode->Prev = nullptr;
	newNode->Data = _data;

	if (_list->Head == nullptr)
	{
		newNode->Next = nullptr;
		newNode->Prev = nullptr;
		_list->Head = newNode;
		_list->Tail = newNode;

		return newNode;
	}
	
	newNode->Prev = _list->Tail;
	_list->Tail->Next = newNode;
	_list->Tail = newNode;

	return newNode;
}

bool MapStruct::WeatherListFirst(GWeatherList * _list)
{
	if (_list->Head == nullptr)
		return false;

	_list->Cur = _list->Head;

	return true;
}

bool MapStruct::WeatherListFirst(GWeatherList * _list, GWeatherData * _data)
{
	if (_list->Head == nullptr)
		return false;

	_list->Cur = _list->Head;

	*_data = _list->Cur->Data;

	return true;
}

bool MapStruct::WeatherListLast(GWeatherList * _list, GWeatherData * _data)
{
	if (_list->Tail == nullptr)
		return false;

	_list->Cur = _list->Tail;
	*_data = _list->Cur->Data;

	return true;
}

bool MapStruct::WeatherListNext(GWeatherList * _list, GWeatherData * _data)
{
	if (_list->Cur->Next == nullptr)
		return false;

	_list->Cur = _list->Cur->Next;
	*_data = _list->Cur->Data;

	return true;
}

bool MapStruct::WeatherListPrev(GWeatherList * _list, GWeatherData * _data)
{
	if (_list->Cur->Prev == nullptr)
		return false;

	_list->Cur = _list->Cur->Prev;
	*_data = _list->Cur->Data;

	return true;
}

int MapStruct::ListCount(GWeatherList * _list)
{
	return _list->NumOfData;
}

GWeatherNode * MapStruct::GetWeatherNode(GWeatherList * _list)
{
	return _list->Cur;
}

void MapStruct::RemoveNode(GWeatherList * _list, GWeatherNode * _node)
{
	if (_list->Head == _node)
	{
		if (_node->Next == nullptr)
		{
			_list->Head = nullptr;
			_list->Tail = nullptr;

			_list->NumOfData--;
			delete(_node);
			return;
		}

		_node->Next->Prev = nullptr;
		_list->Head = _node->Next;

		_list->NumOfData--;
		delete(_node);
		return;
	}

	else if (_list->Tail == _node)
	{
		_node->Prev->Next = nullptr;
		_list->Tail = _node->Prev;

		_list->NumOfData--;
		delete(_node);
		return;
	}

	if(_node->Prev != nullptr)
		_node->Prev->Next = _node->Next;

	if (_node->Next != nullptr)
		_node->Next->Prev = _node->Prev;

	_list->NumOfData--;
	delete(_node);
}

void MapStruct::AnimalDataListInit(GAnimalDataList * _list)
{
	_list->NumOfData = 0;

	_list->Cur = nullptr;
	_list->Head = nullptr;
	_list->Tail = nullptr;
}

GAnimalDataNode * MapStruct::AnimalDataListAdd(GAnimalDataList * _list, GAnimalData _data)
{
	_list->NumOfData++;
	GAnimalDataNode * newNode = new GAnimalDataNode;

	newNode->Next = nullptr;
	newNode->Prev = nullptr;
	newNode->Data = _data;

	if (_list->Head == nullptr)
	{
		newNode->Next = nullptr;
		newNode->Prev = nullptr;
		_list->Head = newNode;
		_list->Tail = newNode;

		return newNode;
	}

	newNode->Prev = _list->Tail;
	_list->Tail->Next = newNode;
	_list->Tail = newNode;

	return newNode;
}

void MapStruct::AnimalDataListAdd(GAnimalDataList * _list, GAnimalDataNode * _node)
{
	_list->NumOfData++;

	_node->Next = nullptr;
	_node->Prev = nullptr;

	if (_list->Head == nullptr ||
		(_list->Head->Data.AnimalCode == 0))
	{
		_node->Next = nullptr;
		_node->Prev = nullptr;
		_list->Head = _node;
		_list->Tail = _node;

		return;
	}

	_node->Prev = _list->Tail;
	_list->Tail->Next = _node;
	_list->Tail = _node;
}

GAnimalDataNode * MapStruct::CreateAnimalDataNode(GAnimalData _data)
{
	GAnimalDataNode * newNode = new GAnimalDataNode;
	newNode->Data = _data;

	return newNode;
}

bool MapStruct::AnimalDataListFirst(GAnimalDataList * _list)
{
	if (_list->Head == nullptr)
		return false;

	_list->Cur = _list->Head;

	return true;
}

bool MapStruct::AnimalDataListLast(GAnimalDataList * _list)
{
	if (_list->Tail == nullptr)
		return false;

	_list->Cur = _list->Tail;

	return true;
}

GAnimalDataNode * MapStruct::AnimalDataListPick(GAnimalDataList * _list)
{
	if (_list->Cur == nullptr)
		return nullptr;

	return _list->Cur;
}

GAnimalDataNode * MapStruct::AnimalDataListNext(GAnimalDataList * _list)
{
	if (_list->Cur->Next == nullptr)
		return nullptr;

	_list->Cur = _list->Cur->Next;

	return _list->Cur;
}

GAnimalDataNode *  MapStruct::AnimalDataListPrev(GAnimalDataList * _list)
{
	if (_list->Cur->Prev == nullptr)
		return nullptr;

	_list->Cur = _list->Cur->Prev;

	return _list->Cur;
}

int MapStruct::ListCount(GAnimalDataList * _list)
{
	return _list->NumOfData;
}

GAnimalDataNode * MapStruct::GetAnimalDataNode(GAnimalDataList * _list)
{
	return _list->Cur;
}

void MapStruct::RemoveNode(GAnimalDataList * _list, GAnimalDataNode * _node)
{

	if (_list->Head == _node)
	{
		if (_node->Next == nullptr)
		{
			_list->Head = nullptr;
			_list->Tail = nullptr;

			_node->Prev = nullptr;
			_node->Next = nullptr;

			_list->NumOfData--;
			return;
		}

		_node->Next->Prev = nullptr;
		_list->Head = _node->Next;

		_node->Prev = nullptr;
		_node->Next = nullptr;

		_list->NumOfData--;
		return;
	}

	else if (_list->Tail == _node)
	{
		_node->Prev->Next = nullptr;
		_list->Tail = _node->Prev;

		_node->Prev = nullptr;
		_node->Next = nullptr;

		_list->NumOfData--;
		return;
	}

	if (_node->Prev != nullptr)
		_node->Prev->Next = _node->Next;

	if (_node->Next != nullptr)
		_node->Next->Prev = _node->Prev;

	_node->Prev = nullptr;
	_node->Next = nullptr;

	_list->NumOfData--;
}

void MapStruct::ItemDataListInit(GItemDataList * _list)
{
	_list->NumOfData = 0;

	_list->Cur = nullptr;
	_list->Head = nullptr;
	_list->Tail = nullptr;
}

GItemDataNode * MapStruct::ItemDataListAdd(GItemDataList * _list, GItemData _data)
{
	_list->NumOfData++;
	GItemDataNode * newNode = new GItemDataNode;

	newNode->Next = nullptr;
	newNode->Prev = nullptr;
	newNode->Data = _data;
	newNode->Data.NodeAdr = newNode;

	if (_list->Head == nullptr)
	{
		newNode->Next = nullptr;
		newNode->Prev = nullptr;
		_list->Head = newNode;
		_list->Tail = newNode;

		return newNode;
	}

	newNode->Prev = _list->Tail;
	_list->Tail->Next = newNode;
	_list->Tail = newNode;

	return newNode;
}

void MapStruct::ItemDataListAdd(GItemDataList * _list, GItemDataNode * _data)
{
	_list->NumOfData++;

	_data->Next = nullptr;
	_data->Prev = nullptr;
	_data->Data.NodeAdr = _data;

	if (_list->Head == nullptr ||
		(_list->Head->Data.ItemCode == 0 && _list->Head->Data.ItemAidCode == 0))
	{
		_data->Next = nullptr;
		_data->Prev = nullptr;
		_list->Head = _data;
		_list->Tail = _data;
		
		return;
	}

	_data->Prev = _list->Tail;
	_list->Tail->Next = _data;
	_list->Tail = _data;
}

GItemDataNode * MapStruct::CreateItemDataNode(GItemData _data)
{
	GItemDataNode * newNode = new GItemDataNode;
	newNode->Data = _data;
	newNode->Data.NodeAdr = newNode;

	return newNode;
}

bool MapStruct::ItemDataListFirst(GItemDataList * _list)
{
	if (_list->Head == nullptr)
		return false;

	_list->Cur = _list->Head;

	return true;
}

bool MapStruct::ItemDataListLast(GItemDataList * _list)
{
	if (_list->Tail == nullptr)
		return false;

	_list->Cur = _list->Tail;

	return true;
}

bool MapStruct::ItemDataListPick(GItemDataList * _list, GItemData * _data)
{
	if (_list->Cur == nullptr)
		return false;

	*_data = _list->Cur->Data;

	return true;
}

bool MapStruct::ItemDataListNext(GItemDataList * _list, GItemData * _data)
{
	if (_list->Cur->Next == nullptr)
		return false;

	_list->Cur = _list->Cur->Next;
	*_data = _list->Cur->Data;

	return true;
}

bool MapStruct::ItemDataListPrev(GItemDataList * _list, GItemData * _data)
{
	if (_list->Cur->Prev == nullptr)
		return false;

	_list->Cur = _list->Cur->Prev;
	*_data = _list->Cur->Data;

	return true;
}

int MapStruct::ListCount(GItemDataList * _list)
{
	return _list->NumOfData;
}

GItemDataNode * MapStruct::GetItemDataNode(GItemDataList * _list)
{
	return _list->Cur;
}

void MapStruct::RemoveNode(GItemDataList * _list, GItemDataNode * _node)
{
	if (_list->Head == _node)
	{
		if (_node->Next == nullptr)
		{
			_list->Head = nullptr;
			_list->Tail = nullptr;

			_node->Prev = nullptr;
			_node->Next = nullptr;

			_list->NumOfData--;
			return;
		}

		_node->Next->Prev = nullptr;
		_list->Head = _node->Next;

		_node->Prev = nullptr;
		_node->Next = nullptr;

		_list->NumOfData--;
		return;
	}

	else if (_list->Tail == _node)
	{
		_node->Prev->Next = nullptr;
		_list->Tail = _node->Prev;

		_node->Prev = nullptr;
		_node->Next = nullptr;

		_list->NumOfData--;
		return;
	}

	if (_node->Prev != nullptr)
		_node->Prev->Next = _node->Next;

	if (_node->Next != nullptr)
		_node->Next->Prev = _node->Prev;

	_node->Prev = nullptr;
	_node->Next = nullptr;

	_list->NumOfData--;
}
