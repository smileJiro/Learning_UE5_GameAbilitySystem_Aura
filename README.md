# ⚡ Learning-UE5-GameAbilitySystem-Aura

언리얼 엔진 5의 Gameplay Ability System(GAS) 숙달을 위한 학습용 프로젝트입니다.  
강의를 따라가며 기초부터 심화까지 직접 구현하며 이론, 디버깅노트 등을 기록하고 있습니다.

## 📌 프로젝트 정보
- **학습 목표**: GAS 아키텍처 이해, C++ 기반의 RPG 프레임워크 구축
- **개발 환경**: UE 5.4+, C++, Rider
- **진행 상황**: Section 7 (진행 중)

---

## 🚀 학습 및 구현 핵심 내용

### 1. GAS 기초 설정 (Section 1-3)
- `Ability System Component`와 `Attribute Set`의 연동
- 캐릭터 및 플레이어 스테이트에 GAS 컴포넌트 이식
- 기본 속성(HP, MP 등) 정의 및 초기화

### 2. 캐릭터 아키텍처 및 입력 (Section 4-5)
- 상속 구조 설계: `CharacterBase` -> `Enemy` / `PlayerCharacter`
- Enhanced Input을 이용한 액션 바인딩

### 3. UI 및 속성 반영 (Section 6-7)
- Common UI 기반의 HUD 위젯 구축
- `AttributeSet`의 값이 변경될 때 UI에 실시간 반영하는 브로드캐스트 시스템 구현
- 데이터 에셋(Data Asset)을 활용한 위젯 컨트롤러 관리

---

## 📅 학습 로그 (log)
- **2025.12.10**: 프로젝트 시작 및 기본 클래스 생성, UE5 AActor와 UComponent 계층 구조 학습
- **2025.12.30**: Enhanced Input 기반 캐릭터 이동 로직 작성
- **2025.12.31**: Gameplay Ability System 구성 요소 학습
- **2026.01.01**: UE5 멀티플레이 환경과 GAS와의 관계 학습
- **2026.01.05**: Gameplay Attribute Data 추가 및 Widget 생성
- **2026.01.08**: GAS Attribute 변경에 따른 UI 데이터 바인딩 구현(MVC Pattern)
- **2026.01.09**: 리슨 서버 2P Widget 데이터 바인딩 버그 해결
- **2026.01.10**: GameplayEffect(GE) 학습 및 HP, MP, Total Potion 액터 개발
- **2026.01.11**: 다양한 GE 정책(Duration Policy, Periodic, Stacking, Modifier 등)을 조작하며 수치 변화와 생명 주기 분석
- **2026.01.12**: GAS Attribute Clamping의 적절한 시점을 찾기위한 GAS 코드 분석
- **2026.01.13**: AbilitySystemComponent.h GAS 코드 분석, GameplayTags 학습

---

## 📑 세부 내용 기록 (Notion)
- https://forest-bowler-224.notion.site/2c59e6fd93be80f79156cf8a9cf2364f?v=2c59e6fd93be8008a949000c9d3cd6d1&source=copy_link
