#include <iostream>
#include <mutex> // using mutex to lock global variable
#include <thread> // thread
#include <stdlib.h> // srand
#include <time.h> // randomize


using namespace std;



mutex m;//lock global var

class Score {
private:
	int PlayerXScore;
	int PlayerYScore;
public:
	Score(int x, int y);//constructor
	int xScore();
	int yScore();
	void setScore(int x, int y);
	void resetScore();
};

void Score::resetScore() {
	
	PlayerXScore = 0;
	PlayerYScore = 0;
	}

Score::Score(int x, int y) {
	
	PlayerXScore = x;
	PlayerYScore = y;
	}
	
void Score::setScore(int x, int y){
	
	PlayerXScore += x;
	PlayerYScore += y;
	}
	
int Score:: xScore() {
	
	return PlayerXScore;
	}
	
int Score::yScore() 
{
	return PlayerYScore;
}

class Pemain {
private:
	 enum Players : char
	{
		PlayerX,
		PlayerY
	} currentPlayer;
	float hits;
public:  
	Pemain(char cek); //constructor
	Pemain(Pemain& clone); //cloning class
	void SetHits(float hits);
	float GetHits();
	char randomPlayer();//random player x or y

	Players GetPlayers() 
	{
		return currentPlayer;
	}

};

char Pemain::randomPlayer() //random a number, if the value is 0, return 'X', else return 'Y'
{
	int random;
	srand(time(0));
	for(int a=0;a<=5;a++){
		random = rand()%4;
		cout << "Acak"<<a<<"="<<random<<endl;
	}
}

Pemain :: Pemain(char check) {
	hits = 0;
	switch (check)
	{
		case 'X': currentPlayer = Players::PlayerX;
			break;
		default : currentPlayer = Players::PlayerY;
			break;
	}
}

Pemain :: Pemain(Pemain& clone) 
{
	hits = clone.hits;
	switch (clone.currentPlayer)
	{
		case Players::PlayerX : currentPlayer = Players::PlayerY;
			break;
		default: currentPlayer = Players::PlayerX;
			break;
	}
}

void Pemain::SetHits(float hits) {
	this->hits = hits;
}

float Pemain:: GetHits() {
	return hits;
}

Score score(0, 0);

void PlayerHit(Pemain* player, bool* isHit) 
{
	//generate random number 0 to 100
	m.lock();
	int* random = new int;
	delete random;
	player->SetHits(rand() % 100);
	m.unlock();

	//show current player
	if (player->GetPlayers() == 0) 
		cout << " Pemain X ";
	else 
		cout << " Pemain Y ";
	cout << "Hit : " << player->GetHits() << " ";

	
	if (player->GetHits() <= 50) 
	{
	
		if (player->GetPlayers() == 0)
			score.setScore(0,1);
		else 
			score.setScore(1,0);
		cout << " \nHit!" << endl;
		*isHit = true;
	}
	else {
		cout << " Repeat" << endl; 
	}
	//delay thread
	this_thread::sleep_for(chrono::seconds(1));
}

int main()
{

	srand(time(NULL));
	
	Pemain playerX('X'); //clone previous player to new player (Deep Copy)
	
	Pemain playerY = playerX;
	startAgain :
	system("");
	score.resetScore();

	while (score.xScore() < 10 && score.yScore() < 10) {
		
		bool isHit = false;
		while (isHit == false) {
			if (playerX.randomPlayer() == 'X') 
			{
				cout << "Start player X : " << endl;
			startagain :
				
				if (isHit == false)
				{
					thread playerXThread(PlayerHit, &playerX, &isHit);
					playerXThread.join();
				}

				if (isHit == false)
				{
					thread playerYThread(PlayerHit, &playerY, &isHit);
					playerYThread.join();
				}

				if (isHit == true) 
					break;
				else
					goto startAgain;

			}
			else
			{
				cout << "Start player Y : " << endl;
			startAgain2 :
				// Pemain Y Thread
				if (isHit == false)
				{
					thread playerYThread(PlayerHit, &playerY, &isHit);
					playerYThread.join();
				}

				// Pemain X Thread
				if (isHit == false)
				{
					thread playerXThread(PlayerHit, &playerX, &isHit);
					playerXThread.join();

				}

				if (isHit == true)
					break;
				else
					goto startAgain2;
			}
		}
	}

	//show winner
	if (score.xScore() >= 10) {
		cout << "\npemain X menang  " << score.xScore() << " - " << score.yScore();
	}
	else {
		cout << "\npemain Y menang  " << score.yScore() << " - " << score.xScore();
	}
	char repeat;
	cout << "\nUlangi ? Y/N " << endl;
	cin >> repeat;
	switch (repeat)
	{
	case 'Y': goto startAgain;
		break;
	case 'y': goto startAgain;
		break;
	default: return 0;
		break;
	}
}
