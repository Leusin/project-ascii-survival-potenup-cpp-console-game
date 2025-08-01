#include "Level.h"

#include <iostream>
#include "Utils/Utils.h"
#include "Actor/Actor.h"

Level::Level()
{
}

Level::~Level()
{
	for (Actor* actor : actors)
	{
		SafeDelete(actor);
	}
}

void Level::BeginPlay()
{
	for (Actor* const actor : actors)
	{
		if (!actor->IsActive() || actor->IsExpired()) // 비활성이거나 폐기 요청중이라면 건너뛰기 
		{
			continue;
		}

		if (actor->HasBegonPlay()) // 이미 호출된 객체 건너뛰기
		{
			continue;
		}

		actor->BeginPlay();
	}
}

void Level::Tick(float deltaTime)
{
	for (Actor* const actor : actors)
	{
		actor->Tick(deltaTime);
	}
}

void Level::Render()
{
	SortActorsBySortingOrder(); // 그리기 전에 정렬

	for (Actor* const actor : actors) // Render Pass
	{
		// 같은 위치에 더 높은 우선순위 액터가 있는지 확인
		Actor* searchActor = nullptr;
		for (Actor* const otherActor : actors)
		{
			if (actor == otherActor)
			{
				continue;
			}

			if (actor->Position() == otherActor->Position())
			{
				if (actor->GetSortingOrder() < otherActor->GetSortingOrder())
				{
					searchActor = otherActor;
					break;
				}
			}
		}

		if (searchActor) // 우선순서가 더 높은 액터가 없음
		{
			continue;
		}

		actor->Render(); // Draw Call
	}
}

void Level::AddActor(Actor* newActor)
{
	// TODO: 중복 여부 확인 예외처리 하면 좋음

	addRequestedActors.emplace_back(newActor);
}

void Level::DestroyActor(Actor* destroyActor)
{
	destroyRequstedActors.emplace_back(destroyActor); // 대기 배열에 추가
}

void Level::ProcessAddAndDestroyActors()
{
	// 삭제할 액터 배열 제외
	for (auto it = actors.begin(); it != actors.end();)
	{
		if ((*it)->IsExpired())
		{
			// erase 함수를 사용하면 iterator가 무효화되기 때문에 
			// 반환되는 값을 저장해야 한다.
			it = actors.erase(it);
			continue;
		}
		++it;
	}

	// 삭제 액터 배열을 순회하면서 delete
	for (auto* actor : destroyRequstedActors)
	{
		// 액터가 그렸던 곳 지우기
		Utils::SetConsolePosition(actor->Position());

		// 콘솔에서 빈문자 출력해서 지우기
		for (int i = 0; i < actor->Width(); ++i)
		{
			printf(" ");
		}

		SafeDelete(actor);
	}

	destroyRequstedActors.clear();

	// 새로운 액터 추가 처리
	for (auto* const actor : addRequestedActors)
	{
		actors.emplace_back(actor);
		actor->SetOwner(this); // 오너 설정.
	}

	addRequestedActors.clear();
}

void Level::SortActorsBySortingOrder()
{
	// TODO: 16개 이하는 삽입 정렬, 퀵정렬, 퀵정렬 떄 패널이 logN 넘으면 힙정렬

	// 거품 정렬
	for (int i = 0; i < (int)actors.size() - 1; ++i)
	{
		for (int j = 0; j < i; ++j)
		{
			if (actors[j]->GetSortingOrder() > actors[j + 1]->GetSortingOrder())
			{
				std::swap(actors[j], actors[j + 1]);
			}
		}
	}
}
