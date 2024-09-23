# URR
![image](https://github.com/user-attachments/assets/f26f1b01-c2ab-41ac-a94e-0b35473b2ba1)

본 프로젝트는 디펜스 게임에 퍼즐 게임 요소를 추가한 프로젝트입니다.


- Engine: UE5.3
- Env: Visual Studio
- 제작기간: 2024.04 ~ 2024.06
- 인원: 1명

</br>
</br>
</br>
</br>
</br>


# Architecture
![URR](https://github.com/user-attachments/assets/15025809-565a-45cc-92c6-bb820d69987c)


</br>
</br>
</br>
</br>
</br>

# Level
## Lobby
![스크린샷 2024-08-30 204437](https://github.com/user-attachments/assets/19a97329-89b2-436e-9e46-10911fe3b3ec)

</br>
</br>

## Game
![스크린샷 2024-08-30 204635](https://github.com/user-attachments/assets/134c8f67-3b04-4631-aa03-e754e60f8b8b)


</br>
</br>
</br>
</br>
</br>

# Lobby
![moveStage](https://github.com/user-attachments/assets/12acde48-040d-4aef-8b04-2ffae2d1ae72)

맵에 존재하는 스테이지 블럭위에 올라가면 해당 스테이지의 정보와 성공 여부를 볼 수 있습니다.
만약, 이전 스테이지를 성공하지 않은 경우 해당 스테이지를 시작할 수 없습니다.
이미 성공한 스테이지는 다시 플레이할 수 있습니다.

이는 UE의 SaveGame을 이용하여 로컬 컴퓨터에 정보를 저장하고 불러옵니다.
각 스테이지 블럭의 스테이지 넘버를 통해 조회합니다.
</br>
</br>
![image](https://github.com/user-attachments/assets/b7debbf9-80a6-43bc-8142-dc4cf912d7b4)


게임이 시작되면 Level에 배치된 StageBlock은 설정된 Stage의 단계에 따라 클리어 여부, 점수 등을 저장한 SaveGame을 Local 컴퓨터에서 탐색합니다.
만약, SaveGame이 존재한다면 클리어 정보를 출력해 줍니다.
SaveGame이 존재하지 않은 가장 낮은 단계의 Stage의 잠금이 해제되며 StageBlock에 올라가게 되면 해당 Stage의 정보를 StageWidget을 통해 확인할 수 있습니다.
StageWidget에서 'Play'버튼을 누르게 되면 GameMode에 게임 시작 요청을 합니다.
GameMode는 Stage에 필요한 정보를 GameInstance에 설정한 후 게임 Level로 이동합니다.

</br>
</br>
</br>

# Gameplay
## Spawn Unit
![spawn](https://github.com/user-attachments/assets/11bf5cb6-7123-4042-9b14-81de921b86b0)
</br>
</br>
</br>
골드를 소모하여 유닛을 소환할 수 있습니다.
1~4 단계의 유닛이 일정한 확률에 의해 소환됩니다.
</br>
![image](https://github.com/user-attachments/assets/eca4cb66-ec52-4bdd-a72c-ffef2554f862)
</br>
</br>
MainUI에 있는 '유닛 스폰'버튼을 클릭하면 PlayerPawn인 Board에게 SpawnAction을 실행하도록 요청합니다.
Board에서는 소유한 ASC에게 Unit을 Spawn하는 Action(GA_Spawn)을 찾아 실행합니다.
GA_Spawn에서는 비동기로 유닛을 소환하는 ActionTask(AT_SpawnUnit)를 생성하고 대기합니다.
AT_SpawnUnit이 실행되면 Board에서 빈 타일을 받아와 해당 타일에 유닛을 생성한 뒤 종료합니다.
</br>
</br>
![can't spwan](https://github.com/user-attachments/assets/f59e8517-4dff-4a15-b69b-829564c981a9)

만약, 타일이 가득찬 상태라면 소환이 불가능합니다.
</br>
</br>
</br>
## Unit Info
![unitInfo](https://github.com/user-attachments/assets/d8b89d85-ed01-43d4-b7d8-fdbb8a27d48d)
</br>
유닛을 클릭하여 유닛의 정보를 확인할 수 있습니다.
사거리가 표시되며 오른쪽에 유닛의 단계, 공격력, 공격 속도 등에 대한 정보가 표시됩니다.
</br>
</br>
![image](https://github.com/user-attachments/assets/726088e5-bcde-43c4-a53f-3b6041e2d6f7)

</br>
</br>
</br>

## Attack

![attack](https://github.com/user-attachments/assets/4b6c0ea0-af89-4e78-be83-5d79429744b0)
</br>
소환된 유닛은 범위 내에 적을 탐지하여 공격합니다.
적을 처치하면 1골드를 획득합니다.
증강을 통해 적 처치시 획득 골드를 증가시킬 수 있습니다.
</br>

![image](https://github.com/user-attachments/assets/cfd40af4-d553-49fd-869e-8635225a92fd)
유닛은 Tick이 실행되면서 공격중인 유닛이 없다면 적을 찾는 Action을 실행합니다.
적을 찾는 Action에서는 TargetActor를 만드는 ActionTask를 실행합니다.
TargetActor는 유닛의 단계마다 다르게 적용됩니다.
예를 들어, 권총을 든 유닛은 가장 가까운 적을 공격하고 지뢰를 든 유닛은 가장 앞에 있는 적을 타깃으로 설정합니다.
TargetActor가 생성이 완료되고 데이터가 설정되었다면 ActionTask는 Callback을 통해 데이터를 전달받습니다.
ActionTask는 UAbilitySystemBlueprintLibrary를 이용하여 event를 발생시켜 Unit에게 Tag를 부여하며 타깃인 적을 설정한 뒤 공격합니다.

</br>
</br>
</br>

## Merge

![move](https://github.com/user-attachments/assets/d3ebb612-a9ba-47bc-8908-b6a8817e45ba)
![merge](https://github.com/user-attachments/assets/86102ab7-130b-4f4c-9232-7e31bb86f74d)
</br>
</br>

한 방향으로 모든 유닛을 이동시킬 수 있습니다.
만약, 이동에 의해 같은 단계의 유닛이 만나게 된다면 한 단계 높은 유닛으로 진화합니다.
각 유닛은 고유한 스탯을 갖고 있습니다.(공격력, 공격속도, 사정거리, 등등..)
만약, 타일이 모두 가득찬 상태에서 어떤 방향으로든 유닛을 이동시켜 공간을 만들 수 없다면 더이상 유닛을 이동시킬 수 없습니다.
</br>
</br>

```
void AURRBoard::MoveLeft()
{
	if (ASC->HasMatchingGameplayTag(URRTAG_PLAYER_ISSPAWNING)) return;

	for (int i = 0; i < 4; i++)
	{
		TArray<int> ExistSet;
		TArray<int> EmptySet;

		for (int j = 0; j < 4; j++)
		{
			if (Tiles[i][j]->IsEmpty()) EmptySet.Add(j);
			else ExistSet.Add(j);
		}

		ExistSet.Heapify();
		EmptySet.Heapify();

		while(!ExistSet.IsEmpty())
		{
			int currentIdx;
			ExistSet.HeapPop(currentIdx, true);
			int currentRank = Tiles[i][currentIdx]->GetRank();

			int prevRank = -1;
			int prevIdx = -1;

			for (int j = currentIdx - 1; j >= 0; j--)
			{
				if (!Tiles[i][j]->IsEmpty())
				{
					prevRank = Tiles[i][j]->GetRank();
					prevIdx = j;
					break;
				}
			}

			if (currentRank == prevRank)
			{
				Tiles[i][currentIdx]->DestroyUnit();
				Tiles[i][prevIdx]->RankUpUnit();

				ExistSet.HeapPush(prevIdx);
			}
			else
			{
				if (EmptySet.IsEmpty()) continue;
				if (currentIdx < EmptySet.HeapTop()) continue;

				int firstEmpty;
				EmptySet.HeapPop(firstEmpty, true);

				Swap(Tiles[i][firstEmpty]->UnitCharacter, Tiles[i][currentIdx]->UnitCharacter);
				Tiles[i][firstEmpty]->AdjustUnit();
				Tiles[i][currentIdx]->AdjustUnit();
			}
		
			EmptySet.HeapPush(currentIdx);
		}
	}
}
```
왼쪽 이동 함수입니다.
이동하는 방향으로 비어있는 칸이 있다면 계속 이동합니다.
즉, 이동하는 쪽으로 쏠린다고 할 수 있습니다.
만약, 같은 단계의 유닛이 있다면 두 유닛은 합쳐져 한 단계 진화합니다.
모든 유닛을 하나씩 옮겨도 되지만 합쳐져 진화한 유닛이 또 다시 진화할 가능성이 있기 때문에 조금 복잡해질 수 있습니다.
이를 해결하기 위해, Heap을 이용해 자신이 이동해야 하는 최종적인 위치를 구해낼 수 있습니다.
최종 위치 이전의 유닛이 존재한다면 같은 단계인지 확인한 후 다음 작업을 이어나갑니다.
만약, 같은 단계의 유닛이 존재한다면 둘 중 하나의 유닛을 파괴하고 남은 유닛은 한 단계 진화시킨 후, 다시 작업해야 하는 Heap에 추가합니다.
모든 유닛을 처리했다면 작업을 마무리합니다.

</br>
</br>
</br>

## Monster Attack
![monsterAttack](https://github.com/user-attachments/assets/3670ab4c-d6c2-472a-bf9b-9d606f4558ca)

특정 경로를 따라 이동하는 적이 죽지 않고 성문앞에 도착하면 성문을 공격합니다.
이때, 적의 남은 체력만큼 피해를 입힙니다.

</br>
</br>
</br>

![image](https://github.com/user-attachments/assets/3bc99a0e-4971-4a07-a6b5-ff61f5ea70ea)
몬스터가 소환된 후 경로를 따라 이동하는 Action을 실행합니다.
해당 Action에서는 Spline을 따라 이동하는 ActionTask(AT_MoveWithSpline)를 생성한 뒤 목적지에 도달하기를 기다립니다.
AT_MoveWithSpline에서는 Tick마다 몬스터의 이동속도에 맞게 계속하여 이동합니다.
만약, 사망하지 않고 목적지(Gate)에 도달했다면 Delegate를 통해 몬스터에게 공격하라는 요청합니다.
몬스터의 공격 Montage가 실행되면, 남아있는 체력만큼 플레이어에게 피해을 입히고 파괴됩니다.

</br>
</br>
</br>

## Augment
![augment](https://github.com/user-attachments/assets/2727a572-1aff-48b2-81f8-a31c108daebb)

각 몬스터 웨이브를 성공적으로 막아냈다면 3개의 증강중 하나를 선택해 상황을 유리하게 바꿀 수 있습니다.
증강의 종류에는 몬스터 처치시 획득하는 골드가 증가하거나 유닛을 소환하는 골드가 감소하는 등 돈과 관련된 증강이 있고, 특정 단계의 유닛의 공격력, 사거리, 공격 속도 등을 증가시키는 증강이 있으며, 특정 단계 혹은 모든 유닛의 단계를 높이는 등의 증강이 존재합니다.
</br>
</br>
![image](https://github.com/user-attachments/assets/8a6ec49e-8e43-4e15-8246-c45445b85955)

</br>
Wave에 포함된 모든 몬스터를 처치하거나 공격을 당해도 생존하였다면 증강을 선택할 수 있는 Widget을 플레이어에게 보여줍니다.
플레이어는 상황에 맞게 유리한 증강을 선택하여 게임을 이어나갈 수 있습니다.
선택된 증강은 WaveManager에게 Callback형태로 전달되어 플레이어의 상태 혹은 유닛에게 적용합니다.

</br>
</br>
</br>


## Clear
![clear](https://github.com/user-attachments/assets/6e76fa8e-b2c9-46ff-b3e7-8b748d9d7635)

마지막 몬스터 웨이브가 종료되었을 때, 성문이 부서지지 않았다면 해당 단계를 통과하여 다음 단계를 진행할 수 있습니다.
</br>
</br>
</br>
## Fail
![fail](https://github.com/user-attachments/assets/9cdd8348-ca8d-4612-a81b-a8f5cf0531c6)

마지막 몬스터 웨이브가 종료되기 전에 성문의 체력이 0이하로 내려간다면 패배하게 됩니다.
다음 단계를 진행하기 위해서는 꼭 이전 단계를 통과해야 합니다.
