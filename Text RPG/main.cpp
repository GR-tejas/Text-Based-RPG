#include <iostream>
#include <cstdlib>
#include <ctime>
#include <set>

using namespace std;

// Item enum class
enum class Item {
    Sword, 
    Shield, 
    Armour, 
    Bow
};

//Character base class
class Character {
public:
    Character(int health, int power)
    {
    }
    int randomDamage[3] = { 18, 21, 25 }; 
    bool isRangedAttack = false, isBlocker = false;
};

// Player class
class Player : public Character {
public:
    int health, maxHealth, meleeDamage, defense, healing;
    bool criticalHit, blocker, lifeSteal, rangedAttack;
    Player(int _health, int _power, int _defense, int _healing) : Character(_health, _power) {
        health = maxHealth = _health;
        meleeDamage = _power;
        defense = _defense;
        healing = _healing;
        criticalHit = blocker = lifeSteal = rangedAttack = false;
    }
    set<Item> collectedItems;

    void addItem(Item item) {
        collectedItems.insert(item);
    }

    void levelUp() {
        maxHealth += 20;
        health = maxHealth; // Reset health to max after level up
        meleeDamage += 5;
        defense += 2;
        healing += 20;
        cout << "\nYou have leveled up to become stronger!\nMax Health = " << maxHealth << "\nDefense = " << defense << "\nDamage increased by 5!\nSelf healing increased by 20\n";
    }

    void heal() {
        health += healing;
        if (health > maxHealth)
            health = maxHealth;
        cout << "You healed! Current health: " << health << endl;
    }

    void unlockAbilities(int level) {
        if (level == 2) {
            addItem(Item::Sword);
            criticalHit = true;
            cout << "You obtained a Sword and unlocked Critical Hit ability!" << endl;
        }
        else if (level == 3) {
            addItem(Item::Shield);
            blocker = true;
            cout << "You obtained a Shield and unlocked Blocker ability!" << endl;
        }
        else if (level == 4) {
            addItem(Item::Armour);
            lifeSteal = true;
            cout << "You obtained Armour and unlocked Life Steal ability!" << endl;
        }
        else if (level == 5) {
            addItem(Item::Bow);
            rangedAttack = true;
            cout << "You obtained a Bow and unlocked Ranged Attack ability!" << endl;
        }
    }

    // Randomly activates special abilities
    int attack() {
        int damage = meleeDamage + randomDamage[rand() % 3];

        if (criticalHit && rand() % 100 < 10) {
            cout << "Critical Hit!" << endl;
            damage *= 2; // Double the melee damage
        }

        if (rangedAttack && rand() % 100 < 10) {
            cout << "Ranged Attack Activated! You will avoid damage in the next enemy attack!" << endl;
            isRangedAttack = true;
        }

        if (blocker && rand() % 100 < 10) {
            cout << "Blocker Activated! You will avoid damage in the next enemy attack!" << endl;
            isBlocker = true;
        }

        if (lifeSteal && rand() % 100 < 10) {
            cout << "Life Steal Activated! You regain some health." << endl;
            health += 10;
            if (health > maxHealth) health = maxHealth;
            damage += 10;
        }

        return damage;
    }
};

// Enemy class
class Enemy : public Character {
public:
    int health, attackPower;

    Enemy(int _health, int _power) : Character(_health, _power) {
        health = _health;
        attackPower = _power;
        cout << "Enemy health = " << health;
    }

    int attack() {
        if (isBlocker)
        {
            isBlocker = false;
            return 0;
        }

        if (isRangedAttack)
        {
            isRangedAttack = false;
            return 0;
        }

        return attackPower + randomDamage[rand() % 3]; // Randomized enemy attack
    }
};

// Game manager class
class GameManager {
public:

    void gameStart()
    {
        //Creating player object
        Player player(100, 10, 5, 20);
        cout << "Welcome to the Adventure Game!" << endl;

        for (int level = 1; level <= 6; level++) {
            playLevel(player, level);

            if (player.health <= 0) {
                cout << "Game Over!" << endl;
                break;
            }
        }
        if (player.health > 0)
        {
            cout << "Final Boss Fight: Murloc!" << endl;
            Enemy murloc(200, 25);
            battle(player, murloc);

            if (player.health > 0) {
                cout << "Congratulations! You have defeated the Murlocs and saved your village!" << endl;
            }
            else {
                cout << "You were defeated by the Murlocs. Game Over!" << endl;
            }
        }
        
    }
    // Function to simulate a battle
    void battle(Player& player, Enemy& enemy) {
        while (player.health > 0 && enemy.health > 0) {
            cout << "\nChoose an action: (1) Attack (2) Heal: ";
            int choice;
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                int damage = player.attack();
                enemy.health -= damage;
                if (enemy.health <= 0) enemy.health = 0;
                cout << "You dealt " << damage << " damage to the enemy. Enemy health: " << enemy.health << endl;
                break;
            }
            case 2:
            {
                player.heal();
                break;
            }
            }

            if (enemy.health > 0) {
                int damage = enemy.attack() - player.defense;
                if (damage < 0) damage = 0;
                player.health -= damage;
                cout << "Enemy dealt " << damage << " damage to you. Your health: " << player.health << endl;
            }

            if (player.health <= 0) {
                cout << "You have been defeated!" << endl;
                break;
            }

            if (enemy.health <= 0) {
                cout << "You defeated the enemy!" << endl;
                break;
            }
        }
    }

    // Game Levels
    void playLevel(Player& player, int level) {
        cout << "\nLevel " << level << " begins!" << endl;

        int numEnemies = level;
        for (int i = 1; i <= numEnemies; i++) {
            cout << "\nEnemy " << i << " appears!" << endl;
            Enemy enemy(50 + (level * 10), 10 + (level * 2)); // Enemy stats scale with level
            battle(player, enemy);
            if (player.health <= 0) {
                break;
            }
        }
        if (player.health > 0)
        {
            cout << "\nLevel " << level << " completed!";
            player.levelUp();
            player.unlockAbilities(level);
        }
    }
};

// Main function
int main() {
    srand(time(0)); // Initialize random seed

    GameManager gm;
    gm.gameStart();

    return 0;
}
