#pragma once

/// <summary>
/// 플레이어 캐릭터와 적 NPC 가 가지게 될 공통적인 필드
/// </summary>
struct CharacterStats 
{
    unsigned int level = 0;;
    float hp;
    float speed;
    float damage;
};
