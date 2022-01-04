#pragma once
#include"../Rimuru/Rimuru.h"
#include"../SkeletonLight/SkeleLight.h"

enum class BossState
{
    STATIONARY,
    PHASE1,
    ONTHRONE,
    PHASE2,
    CASTING,
};

class Boss : public Enemy
{
private:
    const float LIGHTDMG;
    const float HEAVYDMG;
    const float LIGHTFORCE;
    const float HEAVYFORCE;
public:
    Boss(const Aen::Vec3f position = Aen::Vec3f(1.f, 0.f, 3.f), float hp = 100.f);
    virtual~Boss();

    // Inherited via Enemy
    virtual void Update(const float& deltaTime, Player& player) override;

    void SetThronePosition(Aen::Vec3f v);
    void SetThronePosition(float x, float y, float z);
    void MakeMinionsSummonable(bool b);

    std::vector<Rimuru*>& GetMinions();

    const BossState GetBS()const;
    int GetEnemiesToSummon();
    int GetEnemiesInVector() const;
    void EmplaceMinion(Rimuru* e);
    void RemoveMinion(Enemy* e);
    void RemoveMinion(uint16_t i);
    bool GetBossEngage(const bool& engage);

protected:

private:

    void LightAttack(const float& deltatime);
    void BigAttack(const float& deltaTime);
    void SummonSlimes(int amountOfSLimes);
    void Wait(const float duration);
    void RandomCombatEvent();

    void UpdateAttack();


    Aen::Entity* mE_hurtBox;
    Aen::OBBox* mp_hurtBox;
    Player* mp_player;
    bool m_waiting;
    bool engage = false;

    Aen::Animator* m_animator;

    Aen::Vec3f m_direction;
    std::vector<Rimuru*> m_pMinions;

    const float m_MAXHP;
    float m_deltatime;


    Aen::Vec3f m_thronePosition; // TODO: Pair with procedual generation
    bool m_isHurting;
    bool m_isCasting;
    bool m_cantSummonSlimes;
    bool m_slimesWereCasted = false;

    float m_knockBackForce;
    float m_attackDamage;
    float m_speed;
    const float BASESPEED;

    BossState bs;
    uint16_t m_minionsToSummon = 0;
    float m_spawnTimer = 0;
    float m_attackTimer = 0;
    Aen::MeshInstance* mp_meshInst;
};

