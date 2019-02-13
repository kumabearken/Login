#include <iostream>
//#include "multimap.cpp"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

//function prototypes
void UserAllowed(vector <string> authorization);
void Access(string act,string obj,vector <string> auth);
bool IsSenior(vector <string> roleHier);
string GetJunRole(vector <string> senJunRol);

int main()
{
	ifstream useRoles;
	useRoles.open("URA.txt");
	string username;
	string temp;
	string role;
	vector <string> roles;
	bool isUser = false;
	
	cout << "login:";
	getline(cin,username);
	while (getline(useRoles,temp))
	{
		stringstream holder;
		holder << temp;
		holder >> temp;
		if(temp == username)
		{
			isUser=true;
			holder >> role;
			roles.push_back(role);
		}
	}
	if (isUser)
	{
		cout << "Welcome " << username << "!\n";
		UserAllowed(roles);
	}
	else 
	{
		cout << "ERROR: user " << username << " is not in the database!\n";
	}
	useRoles.close();
	cin.get();
	return 0;
}

//=================================================
// UserAllowed
//=================================================
// Gets, parses and tests to see if user inputs cmd 
//	that are valid 
// 
// IN: Vector of strings containing roles user is 
//	assigned to
// OUT: NONE
//=================================================

void UserAllowed(vector <string> authorization)
{
	stringstream temp;
	string action;
	string object;
	
	cout << "cmd> ";
	getline(cin,action);
	temp << action;
	temp >> action >> object;
	if(temp.rdbuf()->in_avail()==0)
		Access(action,object,authorization);
	else 
		cout << "Invalid input\n";
}

//==============================================================
// Access
//==============================================================
// Since user is valid ,tests if cmd inputed is valid.
// If so then tests for whether they have permission 
// to those cmd's.
//
// IN: takes in 2 strings which indicate action and object
//		takes in vector of strings with roles assigned to 
//		user
// OUT: NONE
//============================================================== 
void Access(string act,string obj,vector <string> auth)
{
	string temp;
	string juniorRole;
	ifstream perms;
	bool valid=false;
	bool okay=false;
	bool seniority=false;
	perms.open("PRA.txt");
	seniority=IsSenior(auth);
	
	if(seniority)
		juniorRole=GetJunRole(auth);

	while (getline(perms,temp))
	{
		stringstream holder;
		holder << temp;
		holder >> temp;
		for (int i =0;i < auth.size(); ++i)
		{
			if (auth.at(i)==temp)
				okay=true;
			if(seniority)
				if(temp == juniorRole)
					okay=true;
		}		
		if (okay)
		{
			holder >> temp;
			if(temp==act)
			{
				holder >>temp;
				if(temp == obj)
					valid = true;
			}
		}
	}
	if (valid)
	{
		cout <<	"Access granted by virtue of roles: ";
		for (int i =0;i < auth.size(); ++i)
		{
			cout << auth.at(i) << ' ';		
		}
	}
	else
		cout << "Access denied: you are not authorized to perform this action!";	
	perms.close();
}

//==============================================================
// IsSenior
//==============================================================
// using a reference to roles that the user is assigned
//	tests to see if any of these roles are a senior role
//	as in a RBAC archetype.
//
// IN: Takes in a vector of strings that contains the roles
//	the user is assigned
// OUT: A bool that signifies if senior role or not
//==============================================================
bool IsSenior(vector <string> roleHier)
{
	ifstream hier;
	hier.open("HR.txt");
	string senior;
	string junior;
	string temp;
	bool yes=false;

	while(getline(hier,temp))
	{
		stringstream holder;
		holder << temp;
		holder >> senior >> junior;
		for (int i = 0; i<roleHier.size(); ++i)
		{
			if (roleHier.at(i) == senior)
				yes = true;
		}
	}
	hier.close();
	return yes;
}

//==============================================================
// GetJunRole
//==============================================================
// If there is a senior role then the junior role is 
// found and gives back the Junior role to the caller
// 
//  IN: Takes in a vetor of strings in which the assigned
// 	roles are saved
//  OUT: Returns a string of the junior role the user has
//==============================================================
string GetJunRole(vector <string> senJunRol)
{
	ifstream hier;
	hier.open("HR.txt");
	string senior;
	string junior;
	string temp;
	string answer;

	while(getline(hier,temp))
	{
		stringstream holder;
		holder << temp;
		holder >> senior >> junior;
		for (int i = 0; i<senJunRol.size(); ++i)
		{
			if (senJunRol.at(i) == senior)
				answer=junior;
		}
	}
	hier.close();
	return answer;
}
