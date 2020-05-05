/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2014                              *
 *  Authors: Ulrich Kremer                   *
 *           Hans Christian Woithe           *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

void deleteInst(Instruction *inst){
	if(inst==NULL){return;}
	if(inst->prev!=NULL){
		inst->prev->next=inst->next;
	}
	if(inst->next!=NULL){
		inst->next->prev=inst->prev;
	}
	free(inst);
}

//sets every command after the last WRITE not critical since they don't affect the outcome
void uselessEndCommands(Instruction *head){
	Instruction *temp=head;
	temp= LastInstruction(head);
	while(temp->opcode!=WRITE){
		temp->critical=10;
		temp=temp->prev;
	}
}

//goes through the list to check if critical is not critical(10), if it is, delete from the list
void deleteNonCrit(Instruction *head){
	Instruction *temp2=head;
	Instruction *temp3;
	while(temp2!=NULL){
		if(temp2->critical==10){
			temp3=temp2;
			temp2=temp2->next;
			deleteInst(temp3);
		}else{
			temp2=temp2->next;
		}
	}
}

//shortens the list by getting rid of irrevent commands after the final WRITE
//guarantees WRITE to be at the end of the list
void shortenList(Instruction *head){
	uselessEndCommands(head);
	deleteNonCrit(head);
}

//initialize all the critical (except READ and WRITE) to 10 (non-critical)
//READ and WRITE is always critical
void setCrit(Instruction *head){
	Instruction *temp=head;
	while(temp!=NULL){
		if(!(temp->opcode==READ || temp->opcode==WRITE)){
		temp->critical=10;
		temp=temp->next;
		}else{
			temp->critical=1;
			temp=temp->next;
		}
	}
}

Instruction* find(int target, Instruction* ptr){
	Instruction* temp=ptr->prev;
	while(temp->field1 != target){
		temp=temp->prev;
	}
	temp->critical=1;
	return temp;

}


void origin(Instruction *ptr){
	int find1;
	int find2;
	Instruction *temp;
	Instruction *temp2;
	
	if(ptr->opcode==READ ||ptr->opcode==LOADI){
		return;
	}
	switch(ptr->opcode){
		case LOAD:
		case STORE: 
			find1=ptr->field2;
			temp=find(find1,ptr);
			origin(temp);
			return;
		case ADD:
		case SUB:
		case MUL:
		case AND:
		case OR:
			find1=ptr->field2;
			find2=ptr->field3;
			temp=find(find1,ptr);
			temp2=find(find2,ptr);
			origin(temp);
			origin(temp2);
			return;
		case WRITE:
			find1=ptr->field1;
			temp=find(find1,ptr);
			origin(temp);
		default: return;
	}
}


//1 is critical, 10 is non-critical
int main()
{
	Instruction *head;


	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}
	shortenList(head);
	setCrit(head);

	
	Instruction* lptr= LastInstruction(head);
	while(lptr!=NULL){
		if(lptr->opcode==WRITE){
			origin(lptr);
		}
		lptr=lptr->prev;
	}
	





	deleteNonCrit(head);
	if (head) {
		PrintInstructionList(stdout, head);
		DestroyInstructionList(head);
	}
	return EXIT_SUCCESS;
}


