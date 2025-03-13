# URR

<div align="center">
  <img src="https://github.com/user-attachments/assets/f26f1b01-c2ab-41ac-a94e-0b35473b2ba1" alt="URR Main Image" width="600">
  <p><i>디펜스 게임에 퍼즐 게임 요소를 더한 창의적인 게임 프로젝트</i></p>
</div>

<br><br>

## 📖 프로젝트 개요

URR은 전통적인 디펜스 게임 장르에 퍼즐 게임 요소를 융합한 게임 프로젝트입니다. 타워를 합치고 발전시키는 퍼즐 게임과 디펜스 게임의 요소를 결합해 새로운 게임플레이 경험을 제공합니다.

<br><br>

## 🛠️ 개발 환경

- **Engine**: Unreal Engine 5.3
- **IDE**: Visual Studio
- **제작기간**: 2024.04 ~ 2024.08
- **개발인원**: 1명

<br><br>

## 🏗️ 시스템 아키텍처

<div align="center">
  <img src="https://github.com/user-attachments/assets/15025809-565a-45cc-92c6-bb820d69987c" alt="Architecture Diagram" width="700">
</div>

<br><br>

## 🎮 게임 레벨

### 로비 화면

<div align="center">
  <img src="https://github.com/user-attachments/assets/19a97329-89b2-436e-9e46-10911fe3b3ec" alt="Lobby Screen" width="600">
</div>

<br><br>

### 게임 화면

<div align="center">
  <img src="https://github.com/user-attachments/assets/134c8f67-3b04-4631-aa03-e754e60f8b8b" alt="Game Screen" width="600">
</div>

<br><br>
<br><br>

## 📱 로비 시스템

<div align="center">
  <img src="https://github.com/user-attachments/assets/12acde48-040d-4aef-8b04-2ffae2d1ae72" alt="Stage Movement" width="600">
</div>

### 주요 기능

- **스테이지 진행 시스템**: 맵에 배치된 스테이지 블록 위에 올라가면 해당 스테이지의 정보와 클리어 여부 확인 가능
- **스테이지 잠금 해제**: 이전 스테이지를 성공해야만 다음 스테이지 플레이 가능
- **재도전 기능**: 이미 클리어한 스테이지도 다시 플레이 가능

<br>

### 기술적 구현

<div align="center">
  <img src="https://github.com/user-attachments/assets/b7debbf9-80a6-43bc-8142-dc4cf912d7b4" alt="Lobby System Diagram" width="600">
</div>

- UE의 SaveGame 기능을 활용하여 로컬 컴퓨터에 스테이지 클리어 정보 저장 및 로드
- 게임 시작 시 Level에 배치된 StageBlock이 설정된 Stage 단계에 따라 SaveGame에서 클리어 여부, 점수 등을 탐색
- 플레이어가 StageWidget에서 'Play' 버튼 클릭 시 GameMode에 게임 시작 요청
- GameMode는 Stage 정보를 GameInstance에 설정 후 게임 Level로 이동

<br><br>
<br><br>

## 🎲 게임플레이 시스템

### 유닛 소환

<div align="center">
  <img src="https://github.com/user-attachments/assets/11bf5cb6-7123-4042-9b14-81de921b86b0" alt="Unit Spawning" width="600">
</div>

<br>

- 골드를 소모하여 1~4 단계의 유닛을 확률적으로 소환
- 빈 타일이 없으면 소환 불가능

<br>

<div align="center">
  <img src="https://github.com/user-attachments/assets/eca4cb66-ec52-4bdd-a72c-ffef2554f862" alt="Spawn System Diagram" width="500">
  <img src="https://github.com/user-attachments/assets/f59e8517-4dff-4a15-b69b-829564c981a9" alt="Cannot Spawn" width="500">
</div>

#### 기술적 구현
- MainUI의 '유닛 스폰' 버튼 클릭 → PlayerPawn(Board)에게 SpawnAction 요청
- Board는 소유한 ASC에게 Unit을 Spawn하는 Action(GA_Spawn) 실행
- GA_Spawn은 비동기 ActionTask(AT_SpawnUnit) 생성
- AT_SpawnUnit은 빈 타일에 유닛 생성

<br><br>

### 유닛 정보

<div align="center">
  <img src="https://github.com/user-attachments/assets/d8b89d85-ed01-43d4-b7d8-fdbb8a27d48d" alt="Unit Info" width="600">
</div>

- 유닛 클릭 시 사거리, 단계, 공격력, 공격 속도 등 상세 정보 표시

<div align="center">
  <img src="https://github.com/user-attachments/assets/726088e5-bcde-43c4-a53f-3b6041e2d6f7" alt="Unit Info System" width="500">
</div>

<br><br>

### 유닛 공격

<div align="center">
  <img src="https://github.com/user-attachments/assets/4b6c0ea0-af89-4e78-be83-5d79429744b0" alt="Unit Attack" width="600">
</div>

- 유닛은 범위 내 적을 자동으로 탐지하여 공격
- 적 처치 시 1골드 획득 (증강으로 증가 가능)

<br>

<div align="center">
  <img src="https://github.com/user-attachments/assets/cfd40af4-d553-49fd-869e-8635225a92fd" alt="Attack System Diagram" width="500">
</div>

<br>

#### 기술적 구현
- 유닛의 Tick 실행 시 적을 찾는 Action 실행
- TargetActor 생성 ActionTask 실행
- 유닛 유형별 다른 타겟팅 방식 적용 (권총 - 가까운 적, 지뢰 - 맨 앞의 적)
- UAbilitySystemBlueprintLibrary를 통해 이벤트 발생 → 유닛에 Tag 부여 및 타겟 설정 → 공격

### 유닛 합치기 (Merge)

<div align="center">
  <img src="https://github.com/user-attachments/assets/d3ebb612-a9ba-47bc-8908-b6a8817e45ba" alt="Unit Movement" width="500">
  <img src="https://github.com/user-attachments/assets/86102ab7-130b-4f4c-9232-7e31bb86f74d" alt="Unit Merge" width="500">
</div>

- 한 방향으로 모든 유닛 이동 가능
- 같은 단계의 유닛이 만나면 한 단계 높은 유닛으로 진화
- 각 유닛은 공격력, 공격 속도, 사정거리 등 고유 스탯 보유
- 모든 타일이 가득 차 있고 이동으로 공간 확보 불가 시 더 이상 이동 불가

<br>

#### 기술적 구현 (코드 예시)
```cpp
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

- Heap 자료구조를 활용하여 유닛의 최종 이동 위치 계산
- 같은 단계 유닛 합치기 후 Heap에 다시 추가하여 연쇄 합치기 효과 구현

<br><br>

### 몬스터 공격

<div align="center">
  <img src="https://github.com/user-attachments/assets/3670ab4c-d6c2-472a-bf9b-9d606f4558ca" alt="Monster Attack" width="600">
</div>

- 적이 사망하지 않고 성문에 도달하면 공격
- 적의 남은 체력만큼 성문에 피해를 입힘

<br>

<div align="center">
  <img src="https://github.com/user-attachments/assets/3bc99a0e-4971-4a07-a6b5-ff61f5ea70ea" alt="Monster Attack System" width="500">
</div>

<br>

#### 기술적 구현
- 몬스터 소환 후 Spline 경로 따라 이동 Action 실행
- 이동 ActionTask(AT_MoveWithSpline) 생성
- 목적지(Gate) 도달 시 Delegate를 통해 공격 요청
- 공격 Montage 실행 후 남은 체력만큼 피해를 입히고 파괴

<br><br>

### 증강 시스템

<div align="center">
  <img src="https://github.com/user-attachments/assets/2727a572-1aff-48b2-81f8-a31c108daebb" alt="Augment System" width="600">
</div>

- 각 웨이브 성공적 방어 시 3개의 증강 중 하나 선택 가능
- 증강 종류:
  - 경제: 골드 획득량 증가, 소환 비용 감소 등
  - 유닛 강화: 특정 단계 유닛의 공격력, 사거리, 공격 속도 등 증가
  - 진화: 특정 단계 또는 모든 유닛의 단계 상승

<br>

<div align="center">
  <img src="https://github.com/user-attachments/assets/8a6ec49e-8e43-4e15-8246-c45445b85955" alt="Augment Implementation" width="500">
</div>

<br>

#### 기술적 구현
- 웨이브의 모든 몬스터 처치 또는 생존 시 증강 선택 Widget 표시
- 선택된 증강은 WaveManager에 Callback으로 전달
- 플레이어 상태 또는 유닛에게 적용

<br><br>

### 게임 결과

#### 클리어

<div align="center">
  <img src="https://github.com/user-attachments/assets/6e76fa8e-b2c9-46ff-b3e7-8b748d9d7635" alt="Clear Screen" width="600">
</div>

<br>

- 마지막 웨이브 종료 시 성문이 파괴되지 않

<div align="center">
  <img src="https://github.com/user-attachments/assets/9cdd8348-ca8d-4612-a81b-a8f5cf0531c6" alt="Fail Screen" width="600">
</div>

- 마지막 웨이브 종료 전 성문 체력이 0 이하로 내려가면 실패
- 다음 단계 진행을 위해서는 반드시 이전 단계 클리어 필요
