#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <termio.h>

using namespace std;

// #define getch() (p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, 1 << 20, stdin), p1 == p2) ? EOF : *p1++)
// #define getch() getchar()
char buf[1 << 20], *p1, *p2;

inline bool iswasd(char ch)
{
	if (ch == 'w' || ch == 'W') return 1;
	if (ch == 'a' || ch == 'A') return 1;
	if (ch == 's' || ch == 'S') return 1;
	if (ch == 'd' || ch == 'D') return 1;
	return 0;
}
int getch(void)
{
	struct termios tm, tm_old;
	int fd = 0, ch;
	if (tcgetattr(fd, &tm) < 0)
		return -1;
	tm_old = tm;
	cfmakeraw(&tm);
	if (tcsetattr(fd, TCSANOW, &tm) < 0)
		return -1;
	ch = getchar();
	if (tcsetattr(fd, TCSANOW, &tm_old) < 0)
		return -1;
	return ch;
}

class ChessGame
{
	public:
		int main(void) {
			memset(mp, 0, sizeof mp);
			while (!full() && !win) {
				Print();
				char ch = getch();
				if (iswasd(ch)) MoveCursor(ch);
				else if (ch == 'y') Change();
			}
			Print();
			if (win == 0) puts("NIE");
			if (win == 1) puts("X win!");
			if (win == 2) puts("O win!");
			return 0;
		}
	private:
		int turn = 0;
		int x = 1, y = 1;
		int win = 0;
		char mp[3][3];
		bool full(void)
		{
			for (int i = 0; i < 3; ++i)
				for (int j = 0; j < 3; ++j)
					if (!mp[i][j])
						return 0;
			return 1;
		}
		int Check(void)
		{
			for (int i = 0; i < 3; ++i) {
				bool seccess = 1;
				if (mp[i][0] == 0) continue;
				for (int j = 1; j < 3; ++j) {
					if (mp[i][j] != mp[i][0])
						seccess = 0;
				}
				if (seccess) return mp[i][0] == 'X' ? 1 : 2;
			}
			for (int j = 0; j < 3; ++j) {
				bool seccess = 1;
				if (mp[0][j] == 0) continue;
				for (int i = 1; i < 3; ++i)
					if (mp[i][j] != mp[0][j])
					seccess = 0;
				if (seccess) return mp[0][j] == 'X' ? 1 : 2;
			}
			bool seccess = 1;
			if (mp[0][0] == 0) seccess = 0;
			for (int i = 1; i < 3; ++i)
				if (mp[i][i] != mp[0][0])
					seccess = 0;
			if (seccess) return mp[0][0] == 'X' ? 1 : 2;
			seccess = 1;
			if (mp[0][2] == 0) seccess = 0;
			for (int i = 1; i < 3; ++i)
				if (mp[i][2 - i] != mp[0][2])
					seccess = 0;
			if (seccess) return mp[0][2] == 'X' ? 1 : 2;
			return 0;
		}
		void Print(void)
		{
			system("clear");
			puts("+-+-+-+");
			for (int i = 0; i < 3; ++i) {
				putchar('|');
				for (int j = 0; j < 3; ++j) {
					if (i == x && j == y) {
						if (mp[i][j] == 'O') printf("o|");
						else if (mp[i][j] == 'X') printf("x|");
						else printf("n|");
					}
					else printf("%c|", (mp[i][j] ? mp[i][j] : 'N'));
				}
				puts("");
				puts("+-+-+-+");
			}
		}
		void MoveCursor(char ch) {
			if (ch == 'w' || ch == 'W') {
				x = x + 2;
				x = x > 2 ? x - 3 : x;
			}
			if (ch == 'a' || ch == 'A') {
				y = y + 2;
				y = y > 2 ? y - 3 : y;
			}
			if (ch == 's' || ch == 'S') {
				x = x + 1;
				x = x > 2 ? x - 3 : x;
			}
			if (ch == 'd' || ch == 'D') {
				y = y + 1;
				y = y > 2 ? y - 3 : y;
			}
		}
		void Change()
		{
			if (mp[x][y]) return;
			mp[x][y] = (turn & 1) ? 'O' : 'X';
			++turn;
			win = Check();
		}
};

int main(void)
{
	while (1) {
		bool fail = 0;
		puts("use WASD to move the cursor");
		puts("use y to determind");
		puts("input b to begin");
		puts("input q to quit");
		char ch;
		while ((ch = getch()) != 'b') {
			if (ch == 'q') {
				fail = 1;
				break;
			}
		}
		if (fail) break;
		ChessGame game;
		game.main();
	}
	return 0;
}
