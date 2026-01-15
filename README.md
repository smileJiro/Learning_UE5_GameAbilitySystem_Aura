# ⚡ Learning-UE5-GameAbilitySystem-Aura

언리얼 엔진 5의 Gameplay Ability System(GAS) 숙달을 위한 학습용 프로젝트입니다.  
강의를 따라가며 기초부터 심화까지 직접 구현하며 이론, 디버깅노트 등을 기록하고 있습니다.

## 📌 프로젝트 정보
- **학습 목표**: GAS 아키텍처 이해, C++ 기반의 RPG 프레임워크 구축
- **개발 환경**: UE 5.4+, C++, Rider
- **진행 상황**: Section 7 (진행 중)

---

## 🚀 학습 및 구현 핵심 내용

### 1. 프로젝트 기반 구축 및 입력 시스템 (Section 2)
- **핵심 클래스 설계**: `Character`, `PlayerController`, `PlayerState` 간의 상속 구조 및 역할 정의
- **Enhanced Input**: 유연한 입력 처리를 위해 인핸스드 인풋 시스템을 초기 단계에 통합

### 2. GAS 핵심 구조 설계 (Section 3-4)
- **컴포넌트 연동**: `Ability System Component(ASC)`와 `Attribute Set`의 연동 및 초기화 시점(PossessedBy 등) 분석
- **속성 관리**: Health, Mana 등 기본 Attribute 정의 및 데이터 에셋을 통한 수치 초기화
- **멀티플레이어 대응**: 서버와 클라이언트 간의 GAS 컴포넌트 복제(Replication) 기초 구조 파악
- **환경 최적화**: VS2022에서 Rider로 개발 환경을 전환하여 언리얼 엔진 워크플로우 생산성 향상

### 3. RPG UI 시스템 및 인터랙션 (Section 5-6)
- **MVC 패턴 적용**: 데이터 에셋과 위젯 컨트롤러를 활용해 UI와 데이터 간의 의존성을 낮춘 구조 설계
- **Common UI**: HUD 위젯 구축 및 `AttributeSet` 변화를 UI에 실시간 브로드캐스팅하는 시스템 구현
- **Gameplay Effects (GE)**: Instant, Duration, Periodic 등 다양한 정책을 활용한 스탯 변경 및 상태 이상 메커니즘 검증

### 4. Gameplay Tags, DataTable 활용 및 UI 인터랙션 (Section 7)
- **Gameplay Tags**: 태그 시스템을 활용한 액터 상태 정의 및 어빌리티 발동 조건 제어 로직 구현
- **DataTable Row**: 데이터 테이블 Row에 사용할 구조체 선언 및 연동 작업, Potion 사용 시 GameplayTags + DT Row + Deletgate 기반으로 Widget 렌더링
- **AbilitySystemComponent**: `AbilitySystemComponent.h` 소스 코드 분석을 통해 내부 동작 원리 및 태그 관리 로직 학습

### 5. RPG Attribute System, Attribute Menu (Section 8-9, 진행 중 🏃‍♂️)
- **Initialize Attribute from DT, GE**: Attribute를 초기화하는 다양한 루틴 학습
- **Attribute Based Modifers And Modifier Order of Operations**: Attribute 값 기반 Modifiers 연산 방법 학습 및 연산 순서 규칙 학습
---

## 📅 학습 로그 (log)
- **2025.12.10**: 프로젝트 시작 및 기본 클래스 생성, UE5 AActor와 UComponent 계층 구조 학습
- **2025.12.30**: Enhanced Input 기반 캐릭터 이동 로직 작성
- **2025.12.31**: Gameplay Ability System 구성 요소 학습
- **2026.01.01**: UE5 멀티플레이 환경과 GAS와의 관계 학습
- **2026.01.02 ~ 03**: VisualStudio2022 -> Rider로 개발 환경 변경 (빌드 속도 문제, 통합성 부족 문제, AI 에이전트 확장성 등 개발 효율성 고려)
- **2026.01.05**: Gameplay Attribute Data 추가 및 Widget 생성
- **2026.01.08**: GAS Attribute 변경에 따른 UI 데이터 바인딩 구현(MVC Pattern)
- **2026.01.09**: 리슨 서버 2P Widget 데이터 바인딩 버그 해결
- **2026.01.10**: GameplayEffect(GE) 학습 및 HP, MP, Total Potion 액터 개발
- **2026.01.11**: 다양한 GE 정책(Duration Policy, Periodic, Stacking, Modifier 등)을 조작하며 수치 변화와 생명 주기 분석
- **2026.01.12**: GAS Attribute Clamping의 적절한 시점을 찾기위한 GAS 코드 분석
- **2026.01.13**: AbilitySystemComponent.h GAS 코드 분석, GameplayTags 학습
- **2026.01.14**: GameplayTags + DT Row + Deletgate 기반의 데이터 주도형(Data-Driven) UI 알림 시스템 구현 (포션 사용 등 인게임 이벤트 연출 최적화)
- **2026.01.15**: HealthGlobe Widget GhostGlobe 기능 추가, GAS Attribute Clamping 리팩토링, Attribute 초기화 방법(DataTable, GameplayEffect) 학습,
                  GE, DT를 사용해 Attribute 초기화, Modifiers 연산 순서 규칙 학습

---

## 📑 세부 내용 기록 (Notion)
- https://forest-bowler-224.notion.site/2c59e6fd93be80f79156cf8a9cf2364f?v=2c59e6fd93be8008a949000c9d3cd6d1&source=copy_link
