#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// Structure to hold the course information
struct Course {
	// varibles to store course information
	string courseNumber;
	string courseTitle;
	vector<string> coursePrerequisites;

	//default constructor
	Course() {
		courseNumber;
		courseTitle;
	}

	//constructor
	Course(string courseNumber, string courseTitle, vector<string> coursePrerequisites) {
		this->courseNumber = courseNumber;
		this->courseTitle = courseTitle;
		this->coursePrerequisites = coursePrerequisites;
	}
};

// structure to hold the nodes that will be stored in the BST
struct Node {
	Course course;
	Node* left;
	Node* right;

	//defualt const
	Node() {
		left = nullptr;
		right = nullptr;
	}

	//create Node using bid and default Node to set left and right pointer to nullptr
	Node(Course acourse) : Node() {
		course = acourse;
	}
};

//operator to compare courseNumbers (needed for inserting into BST)
bool compareCourseNumber(string a, string b) {
	int minLen;
	if (a.length() > b.length()) {
		minLen = b.length();
	}
	else {
		minLen = a.length();
	}

	//loops until the end of the smaller value is reached, returning the bool
	for (int i = 0; i < minLen; ++i) {
		if (a[i] != b[i]) {
			// if they are both digit
			if (isdigit(a[i]) && isdigit(b[i])) {
				return a[i] < b[i];
			}
			//they are one digit one letter, or both letters using ACII values
			else {
				return a[i] < b[i];
			}
		}
	}

	//all characters are the same 
	return a.length() > b.length();
}

// each time < is used it defualts here
bool operator<(const Course& left, const Course& right) {
	return compareCourseNumber(left.courseNumber, right.courseNumber);
}

//class for the BST 
class BinarySearchTree {

private:
	Node* root;

	void getCourseList(Node* node);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	//delete tree like in milestone 5
	void insert(Course course);
	void deleteTree(Node* node);
	void getCourseList();
	Course findCourse(string courseNumber);

};

//create empty tree with root
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

// delete tree
BinarySearchTree::~BinarySearchTree() {
	deleteTree(root);
}

// is called via recursion to delete the whole tree
void BinarySearchTree::deleteTree(Node* node) {
	if (node != nullptr) {
		deleteTree(node->left);
		deleteTree(node->right);
		delete(node);
	}
}

// traverse list smaller to largest, passes root
void BinarySearchTree::getCourseList() {
	getCourseList(root);
}

//insert new node into the tree
void BinarySearchTree::insert(Course course) {
   // if root node equal to null ptr. Make the new node the root
	if (root == nullptr) {
		// root is equal to new node bid
		root = new Node(course);
		return;
	}
	// else, find where to add the new node
	else {
		// add Node root and new course
		Node* cur = root;
		Node* newNode = new Node(course);
		//while the node is not a leaf
		while (cur != nullptr) {
			//if the new course is smaller than cur node course, we move left
			if (compareCourseNumber(course.courseNumber, cur->course.courseNumber)) {
				//if the next left course is null, newNode is the left leaf
				if (cur->left == nullptr) {
					cur->left = newNode;
					return;
				}
				//Mode to the left
				cur = cur->left;
			}
			//When newNode course is larger, move right
			else {
				// Once we are all the way right, newNode is added as leaf
				if (cur->right == nullptr) {
					cur->right = newNode;
					return;
				}
				//Mode to the right
				cur = cur->right;
			}
		}

	}
}

void BinarySearchTree::getCourseList(Node* node) {
	//while node is not null
	if (node != nullptr) {

		//traverse left
		getCourseList(node->left);

		//ouput information about course
		cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;

		//traverse right
		getCourseList(node->right);
	}
}

//Finds the users selected course
Course BinarySearchTree::findCourse(string courseNumber) {
	//set current node to root
	Node* cur = root;

	//while not null
	while (cur != nullptr) {
		// if match return course
		if (cur->course.courseNumber == courseNumber) {
			return cur->course;
		}
	
		//courseNumber is smaller than current numver, go left
		if (courseNumber < cur->course.courseNumber) {
			cur = cur->left;
		}

		//go right, courseNumber is larger than cur
		else {
			cur = cur->right;
		}
	}

	// there was no match, return defualt course
	Course course;
	return course;
}

//Load the user data from the input file
void loadData(string csvPath, BinarySearchTree* bst) {
	cout << "Loading CSV file: " << csvPath << endl;

	//open file using path
	ifstream file(csvPath);

	//error opening the file
	if (!file.is_open()) {
		cout << "Error opening: " << csvPath << endl;
	}

	//while there is a next line, read the line and store the input in the line variable
	string line;
	while (getline(file, line)) {
		//create course
		Course course;
		stringstream ss(line);
		vector<string> courseData;
		string singleData;
		
		//seperate the line input by the , in the line and adds each element to the courseData vector
		while (getline(ss, singleData, ',')) {
			courseData.push_back(singleData);
		}

		// not enough data in the line to be considered valid
		if (courseData.size() < 2) {
			cout << "Error in line" << endl;
			continue;
		}

		//only courseNumber and title
		else if (courseData.size() == 2) {
			course.courseNumber = courseData[0];
			course.courseTitle = courseData[1];
		}

		//add remaining data (prerequs) to the vector
		else {
			course.courseNumber = courseData[0];
			course.courseTitle = courseData[1];
			//get all the prerequisites left
			for (int i = 2; i < courseData.size(); ++i) {
				course.coursePrerequisites.push_back(courseData[i]);
			}
		}

		// add the new course object to the bst
		bst->insert(course);

	}

	//close the file
	file.close();

	cout << "Loading complete." << endl;
}

// will display a course and all its prerequisites
void displayCourse(Course course) {
	cout << course.courseNumber << ", " << course.courseTitle << endl;
	cout << "Prerequisites: ";

	//if there are prerequisites
	if ((course.coursePrerequisites).size() >= 0) {
		//print each one
		for (int i = 0; i < course.coursePrerequisites.size(); ++i) {
			cout << course.coursePrerequisites[i];
			if (i + 1 < course.coursePrerequisites.size()) {
				cout << ", ";
			}
		}
		cout << endl;
	}

	// course with no prerequisites
	else {
		cout << "none" << endl;
	}

}

int main() {
	//create variables
	string csvPath;
	string courseId;
	bool filePath = false;

	//create bst
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Course course;
	int choice = 0;
	cout << "Welcome to the course planner!" << endl;

	// continue to ask user for a file name while the they entered cant be open or read
	while (filePath == false) {
		cout << "Enter course data file: ";
		cin >> csvPath;

		ifstream file(csvPath);
			if (file.good()) {
				if (!file.fail()) {
					filePath = true;
					break;
				}
			}

			cout << "Invalid file." << endl;

	}

	//menu loop will continue until they exit
	while (choice != 4) {
		cout << "  1. Load Bids" << endl;
		cout << "  2. Print Course List" << endl;
		cout << "  3. Print Course" << endl;
		cout << "  4. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;

		switch (choice) {

		case 1:

			// Complete the method call to load the bids
			loadData(csvPath, bst);
			break;

		case 2:
			//prints sorted list
			bst->getCourseList();
			break;

		case 3:

			//finds course user enters
			cout << "Enter course number: ";
			cin >> courseId;

			course = bst->findCourse(courseId);

			if (!course.courseNumber.empty()) {
				displayCourse(course);
			}
			else {
				cout << "Course number " << courseId << " not found." << endl;
			}

			break;

		case 4:
			//exit number
			break;

		default:
			//user enters a invalid number
			cout << choice << " is not a valid option." << endl;
		}

		// Clear input buffer
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
	}

	cout << "Good bye." << endl;

	return 0;
}

