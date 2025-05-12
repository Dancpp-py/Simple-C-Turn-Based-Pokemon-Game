#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

class Pokemon{
private:
    string name;
    int health;
    int atk[4];

public:
    Pokemon( string pokemonName, int pokemonHealth, int attack1, int attack2, int attack3, int attack4) {
        name    =   pokemonName;
        health  =   pokemonHealth;
        atk[0]  =   attack1;
        atk[1]  =   attack2;
        atk[2]  =   attack3;
        atk[3]  =   attack4;
    }

    string getName(){
        return name;
    }
    int getHealth(){
        return health;
    }
    int getAttack(int attackIndex){
        if(attackIndex >=0 && attackIndex <4){
            return atk[attackIndex];
        }
        else{
            return 0;
        }
    }
    void takeDamage(int damage){
        health -=damage;
        if( health <0){
            health  =   0;
        }
    }
    void setHealth(int newHealth){
        health  =   newHealth;
    }
    virtual void attack(int attackIndexMove, Pokemon& pokemon){
        int attackDamage    =   getAttack(attackIndexMove);
        int chances =   rand() % 100;
        
        if(chances <10){
            cout<<"            ----------------------------------"<<endl;
            cout<<"            | " <<getName() <<" Landed a critical hit!" <<" |"<<endl;
            cout<<"            ----------------------------------"<<endl;
            attackDamage *= 2;
        }
        else if(chances>10 && chances <20){
            cout<<"                      --------------------"<<endl;
            cout<<"                      | " << getName() <<" Missed!" <<" |"<<endl;
            cout<<"                      --------------------"<<endl;
            attackDamage    =   0;
        }

        pokemon.takeDamage(attackDamage);
        cout<<"            ---------------------------------------"<<endl;
        cout<<"            | " << getName() <<" dealt "<< attackDamage <<" damage to "<< pokemon.getName() <<" |"<<endl;
        cout<<"            ---------------------------------------"<<endl;
    }

    virtual void displayMoves() =   0;

};

class UserPokemon : public Pokemon{
private:
    vector<string> moveNames;

public:
    UserPokemon(string pokemonName, int pokemonHealth, int attack1, int attack2, int attack3, int attack4, vector<string> moves): Pokemon(pokemonName,pokemonHealth,attack1,attack2,attack3,attack4), moveNames(moves){}
    
    void attack(int attackIndexMove, Pokemon& pokemon) override {
        cout<<"You used: "<< moveNames[attackIndexMove] <<"!"<<endl;
        Pokemon ::attack(attackIndexMove, pokemon);
    }
    void displayMoves() override {
        cout<<"=============================================================="<<endl;
        cout<<"|                      CHOOSE YOUR MOVE                      |"<<endl;
        cout<<"=============================================================="<<endl;
        for(int i=0; i<4; i++){
            cout<< i+1 <<". "<< moveNames[i] <<" (Damage: "<< getAttack(i) <<" )"<<endl;
        }
        cout<<"=============================================================="<<endl;
    }
    
};

class EnemyPokemon : public Pokemon{
private:
    vector<string> moveNames;

public:
    EnemyPokemon(string pokemonName, int pokemonHealth, int attack1, int attack2, int attack3, int attack4, vector<string> moves): Pokemon(pokemonName,pokemonHealth,attack1,attack2,attack3,attack4), moveNames(moves){}
    
    void attack(int attackIndexMove, Pokemon& pokemon)  override {
        cout<<"Enemy used: "<<moveNames[attackIndexMove]<< "!" <<endl;
        Pokemon ::attack(attackIndexMove, pokemon);
    }
    void displayMoves() override {
        for(int i=0; i<4; i++){
            cout<<i + 1<<". "<< moveNames[i] <<endl;
        }
    }
};

void displayStatus(Pokemon& userPlayer, Pokemon& enemyPlayer){
    cout<<"\n=============================================================="<<endl;
    cout<<"|                        BATTLE STATUS                       |"<<endl;
    cout<<"=============================================================="<<endl;;
    cout<<"| "<< userPlayer.getName() <<" Health Points: "<< userPlayer.getHealth() <<" VS. "<< enemyPlayer.getName() << " Health Points: "<< enemyPlayer.getHealth() <<" |"<<endl;
    cout<<"=============================================================="<<endl;
}
//M,A,S,A,N

int main(){
    srand(time(0));

    vector<UserPokemon> userTeam    =   {
        UserPokemon("Pikachu", 200, 20, 40, 35, 45, {"Thunderbolt","Quick Attack", "Iron Tail", "Electro Ball"}),
        UserPokemon("Greninja", 200, 20, 30, 50, 45, {"Water Shuriken", "Night Slash", "Hydro Pump", "Bubble"}),
        UserPokemon("Lucario", 200, 45, 55, 30, 35, {"Aura Sphere", "Extreme Speed", "Close Combat", "Bone Rush"}),
        UserPokemon("Charizard", 200, 50, 40, 35, 55, {"FlameThrower", "Dragon Claw", "Air Slash", "Heat Wave"})
    };

    vector<EnemyPokemon> enemyTeam  =   {
        EnemyPokemon("Rayquaza", 200, 60, 55, 35, 30, {"Dragon Ascent", "Outrage", "Air Slash", "Extreme Speed"}),
        EnemyPokemon("Groudon", 200, 60, 55,65, 40, {"Precipice", "Earthquake", "Solar Beam", "Fire Punch"}),
        EnemyPokemon("Mewtwo", 200, 50, 45, 45, 25, {"Psychic", "Shadow Ball", "Aura Sphere", "Hyper Beam"}),
        EnemyPokemon("Latios", 200, 45, 50, 50, 40, {"Luster Purge", "Dragon Pulse", "Psychic", "Mist Ball"})
    };

    int dummyUser   =   0;
    int dummyEnemy  =   0;

    while(dummyUser < userTeam.size() && dummyEnemy < enemyTeam.size()){
        UserPokemon& user   =   userTeam[dummyUser];
        EnemyPokemon& enemy =   enemyTeam[dummyEnemy];

        cout<<"\n=============================================================="<<endl;
        cout<<"|                     "<< user.getName() << " vs. " << enemy.getName() <<"                   |"<<endl;
        cout<<"=============================================================="<<endl;
        
        
        while(user.getHealth() > 0 && enemy.getHealth() > 0){
            displayStatus(user,enemy);
            user.displayMoves();
            int choice;
            cout<<"Enter move number (1-4): ";
            cin>>choice;
            cout<<endl;
            
            if(choice < 1 || choice > 4){
                cout<<"Invalid choice of move"<<endl;
                continue;
            }

            user.attack(choice -1,enemy);

            if(enemy.getHealth() <=0){
                cout<<"\n                       ====================="<<endl;
                cout<<"                       ||"<<enemy.getName() <<" Fainted!"<<"||                      "<<endl;
                cout<<"                       ====================="<<endl;
                dummyEnemy++;
                break;
            }

            int enemyMove   =   rand() % 4;
            enemy.attack(enemyMove, user);

            if(user.getHealth() <= 0){
                cout<<"\n                       ===================="<<endl;
                cout<<"                       ||"<< user.getName() <<" Fainted!"<<"||                     "<<endl;
                cout<<"                       ===================="<<endl;
                dummyUser++;
                break;
            }
        }
    }

    if(dummyUser >= userTeam.size()){
        cout<<"\n--------------------------------------------------------------"<<endl;
        cout<<"=============================================================="<<endl;
        cout<<"||         All your Pokemon have fainted! You lost!"<<"         ||"<<endl;
        cout<<"=============================================================="<<endl;
        cout<<"--------------------------------------------------------------"<<endl;
    }
    else{
        
        cout<<"\n--------------------------------------------------------------"<<endl;
        cout<<"=============================================================="<<endl;
        cout<<"||         You defeated all enemy Pokemon! You won!"<<"         ||"<<endl;
        cout<<"=============================================================="<<endl;
        cout<<"--------------------------------------------------------------"<<endl;
    }

    return 0;
}