#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "tree.c"


typedef struct var{
    char type[15];
    char kind[10];
    char symbol[10];
    int c;
    int args[4];
}var;

typedef struct func{
    char type[10][10];
    int count_arg[4];
}func;

typedef struct scope{
    struct scope *father_scope;
    var **vars;
    int size;
    int args[4];
    struct scope *arg_scope;

}scope;


scope* add_new_scope(scope *current_scope);
var* add_new_var(char* type,char* kind,char* symbol ,int* arg);
void add_var_to_scope(scope *current_scope,var *var);
void multipule_vars_dec(scope* current_scope,node* tree,char* type,char* kind);
void multipule_arg_dec(scope* current_scope,node* tree);
void add_id_for_var(scope* current_scope,node* tree,char* type,char* kind,int* arg);
void start_check(node *tree,int count_main);
void init_scope(node* tree,scope* scope);
void print_scope(scope* scope);
scope* find_r(scope* scope, char* symbol);
int find(scope* scope, char* symbol);
char* get_symbol_type(node *root,char* symbol,scope* scope);
void check_func_args(int* args,node *tree); 
void test(scope* scope);
void des_fun(int* x,node *tree);
void in(int* x, char y,int k);
char* get_func_type(scope* scope);
var* get_symbol_var(node* root,char* symbol,scope* scope);


char* IDoNotKnow(scope* scope){
for(int i=scope->size-1;i>=0;i--){
    if(strcmp(scope->vars[i]->kind,"FUNC")==0){
        return scope->vars[i]->type;
    }
}
IDoNotKnow(scope->father_scope);
}





void check_func_args(int* args,node *tree){
    int j=0,k=0;
    int x[4]={0};
    des_fun(x,tree->sons[1]);
    printf("0: %d |1: %d |2: %d |3: %d \n",args[0],args[1],args[2],args[3]);
    printf("0: %d |1: %d |2: %d |3: %d \n",x[0],x[1],x[2],x[3]);

    for(int i=0;i<4;i++){
        k+=x[i];
        j+=args[i];
    }
    if(k!=j){
        printf("Error: %s the number of arguments in the func call is not equal to the number of arguments in the func definition\n",tree->sons[0]->token);
        exit(1);
    }
    for(int i=0;i<4;i++){
        if(x[i]!=args[i]){
            printf("Error: %s the types of the arguments in the function call do not match the types in the function definition\n",tree->sons[0]->token);
            exit(1);
        }
    }
    
}
void des_fun(int* x,node *tree){
    if(tree->size==2){
        in(x,tree->sons[0]->type[0],0);
    printf("i am end for x: 0-%d 1-%d 2-%d 3-%d %s\n",x[0],x[1],x[2],x[3],tree->token);

        des_fun(x,tree->sons[1]);
    }
    if(tree->size==1){
    printf("i am start3 x: 0-%d 1-%d 2-%d 3-%d %s\n",x[0],x[1],x[2],x[3],tree->token);

        in(x,tree->sons[0]->type[0],0);
    printf("i am end3 x: 0-%d 1-%d 2-%d 3-%d %s\n",x[0],x[1],x[2],x[3],tree->token);

    }
}


int check_bool_type_if(node *tree){
    node *expr = tree->sons[0];
    if(strcmp(expr->token , "==") || strcmp(expr->token,"!=") || strcmp(expr->token , ">") || strcmp(expr->token,"<") || strcmp(expr->token,"<=") || strcmp(expr->token,">=")){
        return 1;
    }
    return 0;
}
void in(int* x, char y,int k){
    if(y=='s')
        x[0]=k+x[0]+1;
    if(y=='i')
        x[1]=k+x[1]+1; 
    if(y=='c')
        x[2]=k+x[2]+1;
    if(y=='r')
        x[3]=k+x[3]+1;

}


char* get_symbol_type(node* root,char* symbol,scope* scope){
     if(scope){
        for(int i=0;i<scope->size;i++){
            if(strcmp(scope->vars[i]->symbol,symbol)==0){
                strcpy(root->type,scope->vars[i]->type) ;
                return scope->vars[i]->type;
            }
        }
     get_symbol_type(root,symbol,scope->father_scope);

     }
    else{
        printf("Error: %s are not defined before they can run",symbol);
        exit(1);
        }
}

var* get_symbol_var(node* root,char* symbol,scope* scope){
     if(scope){
        for(int i=0;i<scope->size;i++){
            if(strcmp(scope->vars[i]->symbol,symbol)==0){
                strcpy(root->type,scope->vars[i]->type) ;
                return scope->vars[i];
            }
        }
     get_symbol_var(root,symbol,scope->father_scope);

     }
    else{
        printf("Error: %s are not defined before they can run",symbol);
        exit(1);
        }
}






char* get_func_type(scope* scope){
     if(scope){
        for(int i=scope->father_scope->size-1;i>0;i--){
            printf("############## %d\n",i);
            if(strcmp(scope->father_scope->vars[i]->kind,"FUNC")==0){
                return scope->father_scope->vars[i]->type;
            }
        }
     }
     return "-1";
}

int find(scope* scope, char* symbol){
   char res[15];
   int c=0;
    if(scope){
        for(int i=0;i<scope->size;i++){
            if(strcmp(scope->vars[i]->symbol,symbol)==0){
                strcpy(res,scope->vars[i]->type);
                c++;
            }
        }
    }
    if(c>1){
        printf("Error: %s declared twice in the same scope\n",symbol);
        exit(1);
    }
    
    return c;
}
void test(scope* scope){
        printf(" type- %s ,kind- %s ,symbol-%s.\n",scope->vars[0]->type,scope->vars[0]->kind,scope->vars[0]->symbol);
    
}
scope* find_r(scope* scope, char* symbol){
    if(scope){
        if(find(scope,symbol)==0){
    test(scope);

            return find_r(scope->father_scope,symbol);
        }
        else{
            test(scope->arg_scope);
            return scope->arg_scope;
        }
    
    }
    
    return NULL;
}
void print_scope(scope* scope){
    printf("scope:\n");
    printf("%d\n",scope->size);
    for(int i=0;i<scope->size;i++){
        printf("id %d: type- %s ,kind- %s ,symbol- %s .\n",i,scope->vars[i]->type,scope->vars[i]->kind,scope->vars[i]->symbol);
        
        
        }
    for(int j=0;j<4;j++)
        printf("idtype: %d,count_arg- %d \n",j,scope->args[j]);
}
    
    //print_scope(scope->father_scope);

scope* add_new_scope(scope* current_scope){
    scope *new_scope = (scope*)malloc(sizeof(scope));
    new_scope->father_scope = current_scope;
    new_scope->vars = NULL;
    new_scope->size = 0;
    new_scope->arg_scope=NULL;

    return new_scope;
}

var* add_new_var(char* type,char* kind,char* symbol ,int* arg){
    var *new_var = (var*)malloc(sizeof(var));
    strcpy(new_var->type,type);
    strcpy(new_var->symbol,symbol);
    strcpy(new_var->kind,kind);
    new_var->c = 0;
    for(int i=0;i<4;i++)
         new_var->args[i]= arg[i];


    return new_var;
}

void add_var_to_scope(scope *current_scope,var *var){
    int o=find(current_scope, var->symbol);
    //o+=find(current_scope->arg_scope, var->symbol);
    if(o>0){
        printf("Error: %s declared twice in the same scope\n",var->symbol);
        exit(1);
    }
    current_scope->size++;
    struct var** temp = (struct var**)malloc(sizeof(struct var*)*current_scope->size);
    for(int i=0; i<(current_scope->size-1);i++){
        temp[i]=current_scope->vars[i];   
    }
    temp[current_scope->size-1]=var;   
    current_scope->vars=temp;
}
void multipule_arg_dec(scope* current_scope,node* tree){
    if(tree){
        if(strcmp(tree->token,"ARG")==0){
                int stam[4]={0};
                add_id_for_var(current_scope,tree->sons[0],tree->sons[2]->type,"ARG",stam);
                multipule_vars_dec(current_scope,tree->sons[1],tree->sons[2]->type,"ARG");
                multipule_arg_dec(current_scope,tree->sons[3]);
            }
    }
}
void multipule_vars_dec(scope* current_scope,node* tree,char* type,char* kind){
    if(tree){
        if(strcmp(tree->token,",")==0){
            int stam[4]={0};
            add_id_for_var(current_scope,tree->sons[0],type,kind,stam);
            multipule_vars_dec(current_scope,tree->sons[1],type,kind);
            }
    }
}

void add_id_for_var(scope* current_scope,node* tree,char* type,char* kind,int* arg){
    struct var* temp = NULL;
    if(tree){
        if(strcmp(kind,"ARG")==0){
            in(current_scope->args,type[0],0);
        }
        if(tree->size==0){
            temp=add_new_var(type,kind,tree->token,arg);
            add_var_to_scope(current_scope,temp);
        }
        else{
            temp=add_new_var(type,kind,tree->sons[0]->token,arg);
            add_var_to_scope(current_scope,temp);

        }
    }
}

void start_check(node *tree,int counter_main){
    if(counter_main!=1){
        printf("%d",counter_main);
        printf("Error: only 1 main function allowd\n");
        exit(1);
    }

    int num=0;
    scope* global = NULL;
    //add_new_scope(global);
    init_scope(tree,global);
    //printf("scope:\n");

}

void init_scope(node* tree,scope* scope){
    struct scope* temp = NULL;
    if(tree){
		if(tree->sons){
            //open blocks

            

            if(strcmp(tree->token,"Code:")==0){
                if(tree->sons[0]){
                    init_scope(tree->sons[0],add_new_scope(scope));
                }
            }

            if(strcmp(tree->token,"functions")==0){
                if(tree->size==2){
                    init_scope(tree->sons[0],scope);
                    init_scope(tree->sons[1],scope);
                }
                else{
                    init_scope(tree->sons[0],scope);

                }
            } 
            

            if(strcmp(tree->token,"FUNCTION1")==0){
                    int stam[4]={0};

                    add_id_for_var(scope,tree,tree->sons[1]->type,"FUNC",stam);
                    init_scope(tree->sons[2],add_new_scope(scope));
                    print_scope(scope);
            }

            if(strcmp(tree->token,"FUNCTION2")==0){
                    int stam[4]={0};
                    temp=add_new_scope(scope);
                    
                    add_id_for_var(temp,tree->sons[2],tree->sons[4]->type,"ARG",stam);
                    multipule_vars_dec(temp,tree->sons[3],tree->sons[4]->type,"ARG");
                    multipule_arg_dec(temp,tree->sons[5]);
                    print_scope(temp);

                    add_id_for_var(scope,tree,tree->sons[6]->type,"FUNC",temp->args);
                    scope->arg_scope=temp;
                    temp->father_scope->vars[temp->father_scope->size-1]->c=temp->args [0]+temp->args[1]+temp->args[2]+temp->args[3];
                    printf("ssss %s ---> %d\n",tree->sons[0]->token,temp->father_scope->vars[temp->father_scope->size-1]->c);
                    init_scope(tree->sons[7],temp);
            }
           

            if(strcmp(tree->token,"WHILE")==0){
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[1],scope);

                if(strcmp(tree->sons[0]->type,"bool")!=0){

                        printf("Error:The expresion type of the while satement must be from bool type\n");
                        exit(1);
                    }
                
            }

            if(strcmp(tree->token,"DO")==0){
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[2],scope);

                if(strcmp(tree->sons[2]->type,"bool")!=0){
                    printf("Error:The expresion type of the do while satement must be from bool type\n");
                    exit(1);
                }
                
            }

            if(strcmp(tree->token,"FOR")==0){
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[1],scope);
                init_scope(tree->sons[2],scope);
                init_scope(tree->sons[4],scope);

                if(strcmp(tree->sons[1]->type,"bool")!=0){
                    printf("Error:The expresion type of the for satement must be from bool type\n");
                    exit(1);
                }
            }

            if(strcmp(tree->token,"IF")==0){ 
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[1],scope);
                if(strcmp(tree->sons[0]->type,"bool")!=0){
                    printf("Error:The expresion type of the if satement must be from bool type\n");
                    exit(1);
                }
                

            }
            if(strcmp(tree->token,"IFELSE")==0){
                        init_scope(tree->sons[0],scope);
                        init_scope(tree->sons[1],scope);
                        init_scope(tree->sons[3],scope);
                        if(strcmp(tree->sons[0]->type,"bool")!=0){
                            printf("Error:The expresion type of the if-else satement must be from bool type\n");
                            exit(1);
                        }
            }
            if(strcmp(tree->token,"blook")==0){
                init_scope(tree->sons[0],add_new_scope(scope));


            }
            
            if( strcmp(tree->token,"statements")==0){
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[1],scope);
            }
            
             if(strcmp(tree->token,"expr1")==0){
                init_scope(tree->sons[0],scope);
                strcpy(tree->type,tree->sons[0]->type);       
            }
            if(strcmp(tree->token,"expr2")==0){
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[1],scope);
                strcpy(tree->type,tree->sons[0]->type);       


                
            }
            
            if(strcmp(tree->token,"&")==0){
                init_scope(tree->sons[0],scope);
                printf("type of & %s    %s\n",tree->sons[0]->type,tree->sons[0]->sons[0]->token);

                if(strcmp(tree->sons[0]->type,"real")==0){
                    strcpy(tree->type,"realpointer");  
                }
                else if(strcmp(tree->sons[0]->type,"char")==0){
                    strcpy(tree->type,"charpointer");  
                }
                else if(strcmp(tree->sons[0]->type,"int")==0){
                    strcpy(tree->type,"intpointer"); 
                    printf("%s kkkkkkkkkkkkk %s\n",tree->type,tree->sons[0]->sons[0]->token);

                }
                else{

                    printf("Error cannot use operator & on %s type\n",tree->type);
                    exit(1);
                }
            }
            if(strcmp(tree->token,"|")==0){
                if(strcmp(get_symbol_type(tree->sons[0],tree->sons[0]->token,scope),"string")!=0  ){
                    printf("Error:The type of the variable on the left side of the %s operator does not match the type of the expression on thr right side\n",tree->sons[0]->token);
                    exit(1);
                }
            }
            if(strcmp(tree->token,"()")==0){
                init_scope(tree->sons[0],scope);
                strcpy(tree->type,tree->sons[0]->type);
                
            }
            if(strcmp(tree->token,"+")==0 || strcmp(tree->token,"-")==0 || strcmp(tree->token,"*")==0 || strcmp(tree->token,"/")==0){
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[1],scope);
                if(strcmp(tree->sons[0]->type,"int")==0 && strcmp(tree->sons[1]->type,"int")==0){
                    strcpy(tree->type,"int");
                }
                else if((strcmp(tree->sons[0]->type,"int")==0 ||strcmp(tree->sons[0]->type,"real")==0 ) && (strcmp(tree->sons[1]->type,"int")==0 || strcmp(tree->sons[1]->type,"real")==0)){
                    strcpy(tree->type,"real");
                }
                else{
                    printf("^^^^^%s  ==== %s\n",tree->sons[0]->type,tree->sons[1]->type);
                    printf("Error:The type of the variable on the left side of the %s operator does not match the type of the expression on thr right side\n",tree->token);
                    exit(1); 
                }


            }
            if(strcmp(tree->token,"<")==0 || strcmp(tree->token,"<=")==0 || strcmp(tree->token,">")==0 || strcmp(tree->token,">=")==0 ){
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[1],scope);
                if((strcmp(tree->sons[0]->type,"int")==0 ||strcmp(tree->sons[0]->type,"real")==0 ) && (strcmp(tree->sons[1]->type,"int")==0 || strcmp(tree->sons[1]->type,"real")==0)){
                    strcpy(tree->type,"bool");
                }
                else{
                    printf("Error:The type of the variable on the left side of the %s operator does not match the type of the expression on thr right side\n",tree->token);
                    exit(1); 
                }
            }
            if(strcmp(tree->token,"==")==0 || strcmp(tree->token,"!=")==0){
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[1],scope);

                if(strcmp(tree->sons[0]->type,tree->sons[1]->type)==0 && strcmp(tree->sons[0]->type,"string")!=0 ){
                    strcpy(tree->type,"bool");
                }
                else{
                    printf("%s equal  %s ---------------\n",tree->sons[0]->type,tree->sons[1]->type);

                    printf("Error:The type of the variable on the left side of the %s operator does not match the type of the expression on thr right side\n",tree->token);
                    exit(1); 
                }
            }
            if(strcmp(tree->token,"!")==0){
                init_scope(tree->sons[0],scope);
                if(strcmp(tree->sons[0]->type,"bool")==0){
                    strcpy(tree->type,"bool");
                }
                else{
                    printf("Error:\n");
                    exit(1); 
                }
                }

            if(strcmp(tree->token,"||")==0 || strcmp(tree->token,"&&")==0 ){
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[1],scope);
                if(strcmp(tree->sons[0]->type,"bool")==0 && strcmp(tree->sons[1]->type,"bool")==0){
                    strcpy(tree->type,"bool");
                }
                else{
                    printf("Error:The type of the variable on the left side of the %s operator does not match the type of the expression on thr right side\n",tree->token);
                    exit(1); 
                }


            }
            if(strcmp(tree->token,"pointer")==0){
                strcpy(tree->type,get_symbol_type(tree->sons[0],tree->sons[0]->token,scope)); 
                printf("problem----> token: %s | type: %s\n",tree->sons[0]->token,tree->type);                   
                if(strcmp(tree->type,"realpointer")==0){
                    strcpy(tree->type,"real");  
                }
                else if(strcmp(tree->type,"charpointer")==0){
                    strcpy(tree->type,"char");  
                }
                else if(strcmp(tree->type,"intpointer")==0){
                    strcpy(tree->type,"int");  
                    printf("problem!!!!!!!! token: %s | type: %s\n",tree->sons[0]->token,tree->type);                   

                }
                else if(strcmp(tree->type,"pointer")==0){
                    strcpy(tree->type,"pointer");  
                }
                else{
                    printf("Error: cannot use operator * on %s type\n",tree->type);
                    exit(1);
                }
                }

            if(strcmp(tree->token,"=")==0){
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[1],scope);
                    printf("%s equal %s\n",tree->sons[0]->type,tree->sons[1]->type);
            
                if(strcmp(tree->sons[0]->type,tree->sons[1]->type)!=0 ){
                     if(!(strcmp("pointer",tree->sons[1]->type)==0 && (strcmp(tree->sons[0]->type,"intpointer")==0 || strcmp(tree->sons[0]->type,"charpointer")==0 || strcmp(tree->sons[0]->type,"realpointer")==0  ))){
                        printf("left: %s | right: %s\n",tree->sons[0]->type,tree->sons[1]->type);
                        printf("%sError the variable type on the left side do not match to the varable type on the left side\n",tree->sons[0]->type);
                        exit(1);
                    }  
                }
            }
            if(strcmp(tree->token,"|=|")==0){
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[1],scope);
                if(strcmp(tree->sons[1]->type,"string")==0 && strcmp(tree->sons[0]->type,"int")==0){
                    strcpy(tree->type,"int");
                }
                else{
                    printf("Error: absulote value operator || can not be operatedon the specific  type\n");
                    exit(1);

                }
            }
            if(strcmp(tree->token,"[]")==0  || strcmp(tree->token,"&[]")==0){
                init_scope(tree->sons[0],scope);
                init_scope(tree->sons[1],scope);
                strcpy(tree->type,get_symbol_type(tree->sons[0],tree->sons[0]->token,scope)); 

                
                printf("+left: %s | right: %s\n",tree->sons[0]->type,tree->sons[1]->type);

                if(strcmp(tree->sons[1]->type,"int")!=0){

                    printf("The type of the expression that appears as an index in [] of a strung is not of type int\n");
                    exit(1);
                }
                if(strcmp(tree->sons[0]->type,"string")!=0){
                    printf("The [] operator is used, which is not a strung \n");
                    exit(1);
                }
                if(strcmp(tree->token,"[]")==0){
                    strcpy(tree->type,"char");
                }
                else{
                    strcpy(tree->type,"charpointer");
                }
            }

            if(strcmp(tree->token,"id")==0){
                strcpy(tree->type,get_symbol_type(tree,tree->sons[0]->token,scope));                    
                
            }


//==================================================================================================
            if(strcmp(tree->token,"function call")==0){
                init_scope(tree->sons[1],scope);
                check_func_args((get_symbol_var(tree->sons[0],tree->sons[0]->token,scope))->args,tree);
                strcpy(tree->type,get_symbol_type(tree->sons[0],tree->sons[0]->token,scope)); 
                printf("####%s |||| ### %s\n ",tree->type,get_symbol_type(tree->sons[0],tree->sons[0]->token,scope));
            }


            if(strcmp(tree->token,"RETURN")==0){
                init_scope(tree->sons[0],scope);
                if(strcmp(get_func_type(scope),"string")==0){
                    printf("Error: The return type string can not be return from a function\n");
                    exit(1);
                }
                if(strcmp(IDoNotKnow(scope->father_scope), tree->sons[0]->type)!=0){
                    printf("Error: %s -> %s The return type of the function do not match to the return type\n",tree->sons[0]->type,IDoNotKnow(scope->father_scope));
                    exit(1);
                }

            }


            //insert to scope
            if(strcmp(tree->token,"VAR1")==0){
                int stam[4]={0};
                add_id_for_var(scope,tree->sons[0],tree->sons[2]->type,"VAR",stam);
                multipule_vars_dec(scope,tree->sons[1],tree->sons[2]->type,"VAR");
                print_scope(scope);

            }

            if(strcmp(tree->token,"VAR2")==0){
                int stam[4]={0};

                printf("%s-----------\n",tree->sons[1]->sons[0]->token);
                add_id_for_var(scope,tree->sons[1],"string","VAR",stam);
                multipule_vars_dec(scope,tree->sons[2],"string","VAR");
                printf("%s\n",tree->token);
                print_scope(scope);


            } 
            if(strcmp(tree->token,"blooks")==0){
                    init_scope(tree->sons[0],add_new_scope(scope));
                    if(tree->sons[1]==NULL){
                        printf("oyyyy!!!!\n");
                    }
                    printf(" |||||%d\n",tree->sons[1]->size);
                    if(tree->size==2){
                        init_scope(tree->sons[1],scope);
                    }
            }        
        }
    }
}

