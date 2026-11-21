
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "Attendance.bak"
#define TEMP_FILE "Temp.bak"

typedef struct
{
    char type;
    int rollNo;
    char name[50];
    char branch[80];
    int semester;
    char subject[100];
    char date[15];
    char status;
} Record;

char branches[4][80] =
{
    "Electronics and Communication Engineering",
    "Computer Science Engineering",
    "Mechanical Engineering",
    "Information Technology"
};

char *eceSubjects[8][6] =
{
    {
        "Engineering Mathematics - I",
        "Engineering Physics",
        "Fundamentals of Computer Programming",
        "Fundamentals of Electrical and Electronics",
        "Professional Communication",
        "Health and Yoga"
    },
    {
        "Engineering Mathematics - II",
        "Data Structure and Algorithms",
        "Electronic Devices and Circuits",
        "Digital Logic and Design",
        "Signals and Systems",
        "Ethics and Human Values for Engineers"
    },
    {
        "Engineering Mathematics - III",
        "Analog Electronic Circuits",
        "Microprocessors and Microcontrollers",
        "Network Analysis and Synthesis",
        "Analog Communication",
        "Programming Lab (MATLAB, Python)"
    },
    {
        "Digital Communication",
        "Digital Signal Processing",
        "Electromagnetic Fields",
        "Control Systems",
        "Data Communication and Networks",
        "Electronic Circuit Design"
    },
    {
        "VLSI Design",
        "Microwave Engineering",
        "Digital Image Processing",
        "Mobile and Wireless Communication",
        "ECE Elective - I",
        "Project Design"
    },
    {
        "Optical Communication",
        "Entrepreneurship Development",
        "Open Elective - I",
        "ECE Elective - II",
        "ECE Elective - III",
        "Project Implementation"
    },
    {
        "Antenna and Wave Propagation",
        "Open Elective - II",
        "ECE Elective - IV",
        "ECE Elective - V",
        "Industrial Training",
        "Project Dissertation"
    },
    {
        "ECE-8001 Industrial Internship / R & D Project",
        "", "", "", "", ""
    }
};

int branchChoice, semesterChoice, subjectChoice;
char selectedBranch[80], selectedSubject[100];

// Input Functions
void inputString(char str[], int size)
{
    fgets(str, size, stdin);
    str[strcspn(str, "\n")] = '\0';
}

int inputInt()
{
    char line[30];
    fgets(line, sizeof(line), stdin);
    return atoi(line);
}

// Select Branch
void selectBranch()
{
int i;
    printf("\n========== SELECT BRANCH ==========\n");

    for(i = 0; i < 4; i++)
        printf("%d. %s\n", i + 1, branches[i]);

    printf("\nEnter Branch Choice: ");
    branchChoice = inputInt();

    if(branchChoice < 1 || branchChoice > 4)
    {
        printf("Invalid Branch Choice!\n");
        exit(0);
    }

    strcpy(selectedBranch, branches[branchChoice - 1]);
}

// Select Semester
void selectSemester()
{
int i;
    printf("\n========== SELECT SEMESTER ==========\n");

    for(i = 1; i <= 8; i++)
        printf("%d. Semester - %d\n", i, i);

    printf("\nEnter Semester Choice: ");
    semesterChoice = inputInt();

    if(semesterChoice < 1 || semesterChoice > 8)
    {
        printf("Invalid Semester!\n");
        exit(0);
    }
}

// Select Subject
void selectSubject()
{
    int count = 0,i;

    printf("\n========== SELECT SUBJECT ==========\n");

    if(branchChoice == 1)
    {
	for( i = 0; i < 6; i++)
        {
            if(strlen(eceSubjects[semesterChoice - 1][i]) > 0)
            {
                printf("%d. %s\n", i + 1,
                       eceSubjects[semesterChoice - 1][i]);
                count++;
            }
        }

        printf("\nEnter Subject Choice: ");
        subjectChoice = inputInt();

        if(subjectChoice < 1 || subjectChoice > count)
        {
            printf("Invalid Subject Choice!\n");
            exit(0);
        }

        strcpy(selectedSubject,
               eceSubjects[semesterChoice - 1][subjectChoice - 1]);
    }
    else
    {
        printf("Enter Subject Name: ");
        inputString(selectedSubject, sizeof(selectedSubject));
    }
}

// Check Student Exists
int studentExists(int roll)
{
    FILE *fp;
    Record r;

    fp = fopen(FILE_NAME, "rb");

    if(fp == NULL)
        return 0;

    while(fread(&r, sizeof(Record), 1, fp) == 1)
    {
        if(r.type == 'S' &&
           r.rollNo == roll &&
           strcmp(r.branch, selectedBranch) == 0 &&
           r.semester == semesterChoice)
        {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// Add New Student
void addStudent()
{
    FILE *fp;
    Record r;

    printf("\n========== ADD NEW STUDENT ==========\n");

    printf("Enter Roll Number: ");
    r.rollNo = inputInt();

    if(studentExists(r.rollNo))
    {
        printf("Student Already Exists!\n");
        return;
    }

    printf("Enter Student Name: ");
    inputString(r.name, sizeof(r.name));

    r.type = 'S';
    strcpy(r.branch, selectedBranch);
    r.semester = semesterChoice;
    strcpy(r.subject, "");
    strcpy(r.date, "");
    r.status = '-';

    fp = fopen(FILE_NAME, "ab");

    if(fp == NULL)
    {
        printf("File Opening Error!\n");
        return;
    }

    fwrite(&r, sizeof(Record), 1, fp);
    fclose(fp);

    printf("\nStudent Added Successfully!\n");
}

// Mark Attendance
void markAttendance()
{
    FILE *fp;
    Record r;
    char status[10];

    printf("\n========== MARK ATTENDANCE ==========\n");

    printf("Enter Roll Number: ");
    r.rollNo = inputInt();

    if(!studentExists(r.rollNo))
    {
        printf("Student Not Found! Add Student First.\n");
        return;
    }

    r.type = 'A';
    strcpy(r.branch, selectedBranch);
    r.semester = semesterChoice;
    strcpy(r.subject, selectedSubject);

    printf("Enter Date (YYYY-MM-DD): ");
    inputString(r.date, sizeof(r.date));

    printf("Enter Attendance (P/A): ");
    inputString(status, sizeof(status));

    r.status = status[0];

    if(r.status == 'p') r.status = 'P';
    if(r.status == 'a') r.status = 'A';

    if(r.status != 'P' && r.status != 'A')
    {
        printf("Invalid Attendance Status!\n");
        return;
    }

    fp = fopen(FILE_NAME, "ab");

    if(fp == NULL)
    {
        printf("File Opening Error!\n");
        return;
    }

    fwrite(&r, sizeof(Record), 1, fp);
    fclose(fp);

    printf("\nAttendance Saved Successfully!\n");
}

// Show Attendance
void showAttendance()
{
    FILE *fp;
    Record r;
    int found = 0;

    fp = fopen(FILE_NAME, "rb");

    if(fp == NULL)
    {
        printf("No Attendance Records Found!\n");
        return;
    }

    printf("\n========== ATTENDANCE REPORT ==========\n");

    printf("Branch   : %s\n", selectedBranch);
    printf("Semester : %d\n", semesterChoice);
    printf("Subject  : %s\n\n", selectedSubject);

    printf("%-12s %-20s %-15s %-10s\n",
           "Roll No", "Name", "Date", "Status");

    printf("---------------------------------------------------------\n");

    while(fread(&r, sizeof(Record), 1, fp) == 1)
    {
        if(r.type == 'A' &&
           r.semester == semesterChoice &&
           strcmp(r.branch, selectedBranch) == 0 &&
           strcmp(r.subject, selectedSubject) == 0)
        {
            char studentName[50] = "Unknown";
            FILE *fs;
            Record s;

            fs = fopen(FILE_NAME, "rb");

            if(fs != NULL)
            {
                while(fread(&s, sizeof(Record), 1, fs) == 1)
                {
                    if(s.type == 'S' &&
                       s.rollNo == r.rollNo &&
                       strcmp(s.branch, r.branch) == 0 &&
                       s.semester == r.semester)
                    {
                        strcpy(studentName, s.name);
                        break;
                    }
                }
                fclose(fs);
            }

            printf("%-12d %-20s %-15s %-10c\n",
                   r.rollNo, studentName, r.date, r.status);

            found = 1;
        }
    }

    if(!found)
        printf("No Records Available!\n");

    fclose(fp);
}

// Update Attendance
void updateAttendance()
{
    FILE *fp, *temp;
    Record r;
    int roll, found = 0;
    char date[15], status[10];

    printf("\n========== UPDATE ATTENDANCE ==========\n");

    printf("Enter Roll Number: ");
    roll = inputInt();

    printf("Enter Date (YYYY-MM-DD): ");
    inputString(date, sizeof(date));

    printf("Enter New Status (P/A): ");
    inputString(status, sizeof(status));

    if(status[0] != 'P' && status[0] != 'p' &&
       status[0] != 'A' && status[0] != 'a')
    {
        printf("Invalid Status!\n");
        return;
    }

    fp = fopen(FILE_NAME, "rb");

    if(fp == NULL)
    {
        printf("File Not Found!\n");
        return;
    }

    temp = fopen(TEMP_FILE, "wb");

    if(temp == NULL)
    {
        fclose(fp);
        printf("Temporary File Error!\n");
        return;
    }

    while(fread(&r, sizeof(Record), 1, fp) == 1)
    {
        if(r.type == 'A' &&
           r.rollNo == roll &&
           r.semester == semesterChoice &&
           strcmp(r.branch, selectedBranch) == 0 &&
           strcmp(r.subject, selectedSubject) == 0 &&
           strcmp(r.date, date) == 0)
        {
            r.status = (status[0] == 'p' || status[0] == 'P')
                       ? 'P' : 'A';

            found = 1;
        }

        fwrite(&r, sizeof(Record), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    if(remove(FILE_NAME) != 0)
    {
        printf("Unable to replace attendance file!\n");
        remove(TEMP_FILE);
        return;
    }

    if(rename(TEMP_FILE, FILE_NAME) != 0)
    {
        printf("Unable to rename temporary file!\n");
        return;
    }

    if(found)
        printf("\nAttendance Updated Successfully!\n");
    else
        printf("\nAttendance Record Not Found!\n");
}

// NEW FUNCTION: Show Student List
void showStudentList()
{
    FILE *fp;
    Record r;
    int count = 0;

    fp = fopen(FILE_NAME, "rb");

    if(fp == NULL)
    {
        printf("\nStudent Record File Not Found!\n");
        return;
    }

    printf("\n============================================\n");
    printf("              STUDENT LIST\n");
    printf("============================================\n");

    printf("Branch   : %s\n", selectedBranch);
    printf("Semester : %d\n\n", semesterChoice);

    printf("%-12s %-25s\n", "Roll No", "Student Name");
    printf("--------------------------------------------\n");

    while(fread(&r, sizeof(Record), 1, fp) == 1)
    {
        if(r.type == 'S' &&
           strcmp(r.branch, selectedBranch) == 0 &&
           r.semester == semesterChoice)
        {
            printf("%-12d %-25s\n",
                   r.rollNo, r.name);

            count++;
        }
    }

    printf("--------------------------------------------\n");
    printf("Total Students: %d\n", count);

    fclose(fp);
}

// Main Function
int main()
{
    int choice;

    selectBranch();
    selectSemester();
    selectSubject();

    do
    {
        printf("\n\n========================================\n");
        printf("       STUDENT ATTENDANCE SYSTEM\n");
        printf("========================================\n");

        printf("Branch   : %s\n", selectedBranch);
        printf("Semester : %d\n", semesterChoice);
        printf("Subject  : %s\n", selectedSubject);

        printf("\n-------------- MAIN MENU --------------\n");

        printf("\n1. Add New Student");
        printf("\n2. Mark Attendance");
        printf("\n3. Show Attendance");
        printf("\n4. Update Attendance");
        printf("\n5. Exit Program");
        printf("\n6. Show Student List");

        printf("\n\nEnter Your Choice: ");
        choice = inputInt();

        switch(choice)
        {
            case 1:
                addStudent();
                break;

            case 2:
                markAttendance();
                break;

            case 3:
                showAttendance();
                break;

            case 4:
                updateAttendance();
                break;

            case 5:
                printf("\nThank You! Program Closed.\n");
                break;

            case 6:
                showStudentList();
                break;

            default:
                printf("\nInvalid Choice! Try Again.\n");
        }

    } while(choice != 5);

    return 0;
}
