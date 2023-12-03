#ifndef LOGIN_H
#define LOGIN_H

int loginUser();

const char *subjects[9] = {"Client", "Premium Client", "Employee", 
                            "Financial Planner", "Financial Advisor", "Investment Analyst", 
                            "Technical Support", "Teller", "Compliance Officer"
                            };


const char *objects[9] = {"Client Account Balance", "Client Investments Portfolio", "Client Information", 
                            "Financial Advisor Details", "Investment Analyst Details", "Money Market Instruments",
                             "Private consumer instruments", "Derivatives trading", "Interest instruments"
                            };

const int NUM_OBJECTS = 9;


#endif