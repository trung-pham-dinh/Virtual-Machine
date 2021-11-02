#include "VM.h"

regex ins_2_op("^[^ ]+ [^ ]+, [^ ]+$");
regex ins_1_op("^[^ ]+ [^ ]+$");
regex op_d_s("^\\w+ R([1-9]|1[0-5]), (\\d+(\\.\\d+)?|(0|[1-9]|[1-9]\\d+)A|R([1-9]|1[0-5])|true|false)$");
regex op_s("^\\w+ (\\d+(\\.\\d+)?|(0|[1-9]|[1-9]\\d+)A|R([1-9]|1[0-5])|true|false)$");
regex op_d("^\\w+ R([1-9]|1[0-5])$");
regex opr2("(Add|Minus|Mul|Div|CmpEQ|CmpNE|CmpLT|CmpLE|CmpGT|CmpGE|And|Or|Move|Load|Store|JumpIf) .*");

regex getop2("^[^ ]+ R(\\d+), ([^ ]+)$");

regex regi("^R(\\d+)$");
regex addr("^(\\d+)A$");
regex real("^(\\d+\\.\\d+)$");
regex integer("^(\\d+)$");

void Stack::push(int val) {
    temp = new Node(val);
    temp->next = head;
    head = temp;
    num++;
}
int Stack::pop() {
    int addr = head->val;
    temp = head;
    head = head->next;
    delete temp;
    num--;
    return addr;
}
int Stack::top() {
    return head->val;
}
bool Stack::isEmpty() {
    return (head)? false:true;
}
bool Stack::isFull() {
    return (num>=1000)? true:false;
}
void Stack::print() {
    iter = head;
    while(iter) {
        cout<<iter->val<<" ";
        iter = iter->next;
    }
}




void VM::run(string filename) {
    compile_error(filename);
    //printCMD();
    bool is_halt = 0;
    while(!is_halt) {
        //cout<<"IP: "<<IP<<endl;
        if(IP > numCMD-1) break;
        readCMD();
        IP++;
        executeCMD(is_halt);
        //printReg();
    }
}
void VM::compile_error(string filename) {
    ifstream file(filename);
    string line;

    while(!file.eof()) {
        getline(file, line);
        numCMD++;
    }
    
    file.clear(); // clear failbit
    file.seekg(0);
    
    memBank = new CMD[numCMD];
    if(file.is_open()) {
        int icmd = 0;
        while(getline(file,line)) {
            if(!instruction_error(line)) {
                file.close();
                throw InvalidInstruction(icmd);
                //cout<<"Invalid instruction at address: "<<icmd<<endl;
            }
            else if(!operand_error(line)) {
                file.close();
                throw InvalidOperand(icmd);
                //cout<<"Invalid operand at address: "<<icmd<<endl;
            }
            else {
                storeCMD(line,icmd);
                //cout<<"Correct at: "<<icmd<<endl;
            }
            icmd++;
        }
        file.close();
    }

}
bool VM::instruction_error(const string &line) {
    string code = line.substr(0, line.find(' '));
    if(regex_match(line, opr2))
    {
        if(regex_match(line, ins_2_op)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if(code == "Not" ||code == "Jump" ||code == "Call" ||code == "Input" ||code == "Output")
    {
        if(regex_match(line, ins_1_op)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if(code == "Return" || code == "Halt") {
        if(line == "Return" || line == "Halt")
            return true;
        else 
            return false;
    }
    return false;
}
bool VM::operand_error(const string &line) {
    string code = line.substr(0, line.find(' '));
    if(regex_match(line, opr2))
    {
        if(regex_match(line, op_d_s)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if(code == "Jump" ||code == "Call" ||code == "Output")
    {
        if(regex_match(line, op_s)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if(code == "Not" || code == "Input")
    {
        if(regex_match(line, op_d)) {
            return true;
        }
        else {
            return false;
        }
    }
    else if(code == "Halt" ||code == "Return")
    {
        return true;
    }
    return false;
}

void VM::storeCMD(const string& line, int icmd) {
    string code = line.substr(0, line.find(' '));
    
    if(code == "Add") {
        memBank[icmd].code = Add;
        extract2op(line,icmd);
    }
    else if(code == "Minus") {
        memBank[icmd].code = Minus;
        extract2op(line,icmd);
    }
    else if(code == "Mul") {
        memBank[icmd].code = Mul;
        extract2op(line,icmd);
    }
    else if(code == "Div") {
        memBank[icmd].code = Div;
        extract2op(line,icmd);
    }
    else if(code == "CmpEQ") {
        memBank[icmd].code = CmpEQ;
        extract2op(line,icmd);
    }
    else if(code == "CmpNE") {
        memBank[icmd].code = CmpNE;
        extract2op(line,icmd);
    }
    else if(code == "CmpGE") {
        memBank[icmd].code = CmpGE;
        extract2op(line,icmd);
    }
    else if(code == "CmpGT") {
        memBank[icmd].code = CmpGT;
        extract2op(line,icmd);
    }
    else if(code == "CmpLE") {
        memBank[icmd].code = CmpLE;
        extract2op(line,icmd);
    }
    else if(code == "CmpLT") {
        memBank[icmd].code = CmpLT;
        extract2op(line,icmd);
    }
    else if(code == "Not") {
        memBank[icmd].code = Not;
        extract1op(line,icmd,1);
    }
    else if(code == "And") {
        memBank[icmd].code = And;
        extract2op(line,icmd);
    }
    else if(code == "Or") {
        memBank[icmd].code = Or;
        extract2op(line,icmd);
    }
    else if(code == "Move") {
        memBank[icmd].code = Move;
        extract2op(line,icmd);
    }
    else if(code == "Load") {
        memBank[icmd].code = Load;
        extract2op(line,icmd);
    }
    else if(code == "Store") {
        memBank[icmd].code = Store;
        extract2op(line,icmd);
    }
    else if(code == "Jump") {
        memBank[icmd].code = Jump;
        extract1op(line,icmd,0);
    }
    else if(code == "JumpIf") {
        memBank[icmd].code = JumpIf;
        extract2op(line,icmd);
    }
    else if(code == "Call") {
        memBank[icmd].code = Call;
        extract1op(line,icmd,0);
    }
    else if(code == "Input") {
        memBank[icmd].code = Input;
        extract1op(line,icmd,1);
    }
    else if(code == "Output") {
        memBank[icmd].code = Output;
        extract1op(line,icmd,0);
    }
    else if(code == "Return") {
        memBank[icmd].code = Return;
    }
    else if(code == "Halt") {
        memBank[icmd].code = Halt;
    }
}
void VM::extract2op(const string& line, int icmd) {
    smatch match;
    smatch temp;
    
    regex_search(line, match, getop2);
        
    memBank[icmd].reg = stoi(match.str(1), NULL);
    
    string op2 = match.str(2);
    if(op2 == "true" || op2 == "false") {
        memBank[icmd].src.type = is_bool;
        memBank[icmd].src.val.ival = (op2 == "true")? 1:0;
    }
    else if(regex_search(op2, temp, addr)) {
        memBank[icmd].src.type = is_addr;
        memBank[icmd].src.val.ival = stoi(temp.str(1), NULL);
    }
    else if(regex_search(op2, temp, real)) {
        memBank[icmd].src.type = is_real;
        memBank[icmd].src.val.rval = stod(temp.str(1), NULL);
    }
    else if(regex_search(op2, temp, integer)) {
        memBank[icmd].src.type = is_int;
        memBank[icmd].src.val.ival = stoi(temp.str(1), NULL);
    }
    else if(regex_search(op2, temp, regi)) {
        memBank[icmd].src.type = is_reg;
        memBank[icmd].src.val.ival = stoi(temp.str(1), NULL);
    }
}
void VM::extract1op(const string& line, int icmd, bool is_dest) {
    regex getopdest("[^ ]+ R(\\d+)");
    regex getop1("^[^ ]+ ([^ ]+)$");
    smatch match;
    smatch temp;
    if(is_dest) {
        regex_search(line, match, getopdest);
        memBank[icmd].reg = stoi(match.str(1), NULL);
    }
    else {
        regex_search(line, match, getop1);
        
        string op2 = match.str(1);
        if(op2 == "true" || op2 == "false") {
            memBank[icmd].src.type = is_bool;
            memBank[icmd].src.val.ival = (op2 == "true")? 1:0;
        }
        else if(regex_search(op2, temp, addr)) {
            memBank[icmd].src.type = is_addr;
            memBank[icmd].src.val.ival = stoi(temp.str(1), NULL);
        }
        else if(regex_search(op2, temp, real)) {
            memBank[icmd].src.type = is_real;
            memBank[icmd].src.val.rval = stod(temp.str(1), NULL);
        }
        else if(regex_search(op2, temp, integer)) {
            memBank[icmd].src.type = is_int;
            memBank[icmd].src.val.ival = stoi(temp.str(1), NULL);
        }
        else if(regex_search(op2, temp, regi)) {
            memBank[icmd].src.type = is_reg;
            memBank[icmd].src.val.ival = stoi(temp.str(1), NULL);
        }
    }
}

void VM::readCMD() {
    Type destType = getDestType(IP);
    Type srcType = getSrcType(IP);
    
    switch(memBank[IP].code) {
    case Div:
        if(getSrcInt(IP) == 0) {
            throw DivideByZero(IP);
        }
    case Add:
    case Minus:
    case Mul:
        if(!(destType==is_real || destType==is_int)
         ||!(srcType ==is_real || srcType==is_int)) {
            throw TypeMismatch(IP);
         }
        break;
    case CmpEQ:
    case CmpNE:
    case CmpLT:
    case CmpLE:
    case CmpGT:
    case CmpGE:
        if(!((destType == is_bool && srcType == is_bool)  ||  ((destType==is_int||destType==is_real) && (srcType==is_int||srcType==is_real)))){
            throw TypeMismatch(IP);
        }
        break;
    case Not:
        if(destType != is_bool) {
            throw TypeMismatch(IP);
        }
        break;
    case And:
    case Or:
        if(!(destType == is_bool && srcType == is_bool)) {
            throw TypeMismatch(IP);
        }
        break;
    case Move:
        break;
    case Load:
        if(!(srcType == is_addr)) {
            throw TypeMismatch(IP);
        }
        break;
    case Store:
        if(!(destType == is_addr)) {
            throw TypeMismatch(IP);
        }
        break;
    case JumpIf:
        if(!(destType == is_bool)) {
            throw TypeMismatch(IP);
        }
        if(getDestInt(IP)!= 0 && (getSrcInt(IP) < 0 || getSrcInt(IP) > numCMD-1)) {
            throw InvalidDestination(IP);
        }
    case Jump:
    case Call:
        if(!(srcType == is_addr)) {
            throw TypeMismatch(IP);
        }
        if(memBank[IP].code != JumpIf &&(getSrcInt(IP) < 0 || getSrcInt(IP) > numCMD-1)) {
            throw InvalidDestination(IP);
        }
        if(memBank[IP].code == Call && reAdStack.isFull()) {
            throw StackFull(IP);
        }
        break;
    case Input:
        if(destType == is_addr) {
            throw InvalidInput(IP);
        }
        break;
    default:
        break;
    }
}
void VM::executeCMD(bool& is_halt) {
    Code code = memBank[IP-1].code;
    switch(code) {
    case Add:
    case Minus:
    case Mul:
    case Div:
        arithmetic();
        break;
    case CmpEQ:
    case CmpGE:
    case CmpGT:
    case CmpLE:
    case CmpLT:
    case CmpNE:
        compare();
        break;
    case Not:
        getDestInt(IP-1) = !getDestInt(IP-1);
        break;
    case And:
        getDestInt(IP-1) = (getDestInt(IP-1) & getSrcInt(IP-1));
        break;
    case Or:
        getDestInt(IP-1) = (getDestInt(IP-1) | getSrcInt(IP-1));
        break;
    case Move:
        MoveData();
        break;
    case Store:
        StoreData();
        break;
    case Load:
        LoadData();
        break;
    case Jump:
        IP = getSrcInt(IP-1);
        break;
    case JumpIf:
        if(getDestInt(IP-1)) IP = getSrcInt(IP-1);
        break;
    case Call:
        reAdStack.push(IP);
        IP = getSrcInt(IP-1);
        break;
    case Return:
        IP = reAdStack.pop();
        break;
    case Halt:
        is_halt = 1;
        break;
    case Output:
        OutputData();
        break;
    case Input:
        InputData();
        break;
    default:
        break;
    }
}

void VM::arithmetic() {
    Type typeDest = getDestType(IP-1);
    Type typeSrc = getSrcType(IP-1);
    
    bool isDestInt = (typeDest == is_int);
    bool isSrcInt = (typeSrc == is_int);
    
    if(isDestInt && isSrcInt) {
        getDestType(IP-1) = is_int;
        switch(memBank[IP-1].code) {
        case Add:
            getDestInt(IP-1) = getDestInt(IP-1) + getSrcInt(IP-1);
            break;
        case Minus:
            getDestInt(IP-1) = getDestInt(IP-1) - getSrcInt(IP-1);
            break;
        case Mul:
            getDestInt(IP-1) = getDestInt(IP-1) * getSrcInt(IP-1);
            break;
        case Div:
            getDestInt(IP-1) = getDestInt(IP-1) / getSrcInt(IP-1);
            break;
        default:
            break;
        }
        
    }
    else {
        getDestType(IP-1) = is_real;
        switch(memBank[IP-1].code) {
        case Add:
            getDestReal(IP-1) = 1.0*((isDestInt)? getDestInt(IP-1):getDestReal(IP-1)) + ((isSrcInt)? getSrcInt(IP-1):getSrcReal(IP-1));
            break;
        case Minus:
            getDestReal(IP-1) = 1.0*((isDestInt)? getDestInt(IP-1):getDestReal(IP-1)) - ((isSrcInt)? getSrcInt(IP-1):getSrcReal(IP-1));
            break;
        case Mul:
            getDestReal(IP-1) = 1.0*((isDestInt)? getDestInt(IP-1):getDestReal(IP-1)) * ((isSrcInt)? getSrcInt(IP-1):getSrcReal(IP-1));
            break;
        case Div:
            getDestReal(IP-1) = 1.0*((isDestInt)? getDestInt(IP-1):getDestReal(IP-1)) / ((isSrcInt)? getSrcInt(IP-1):getSrcReal(IP-1));
            break;
        default:
            break;
        }
    }
}
void VM::compare() {
    Type typeDest = getDestType(IP-1);
    Type typeSrc = getSrcType(IP-1);
    
    bool isDestInt = (typeDest == is_int);
    bool isSrcInt = (typeSrc == is_int);
    
    getDestType(IP-1) = is_bool;
    if(typeDest == is_bool && typeSrc == is_bool) {
        switch(memBank[IP-1].code) {
        case CmpEQ:
            getDestInt(IP-1) = (getDestInt(IP-1)==getSrcInt(IP-1))? 1:0;
            break;
        case CmpNE:
            getDestInt(IP-1) = (getDestInt(IP-1)!=getSrcInt(IP-1))? 1:0;
            break;
        case CmpLT:
            getDestInt(IP-1) = (getDestInt(IP-1)<getSrcInt(IP-1))? 1:0;
            break;
        case CmpLE:
            getDestInt(IP-1) = (getDestInt(IP-1)<=getSrcInt(IP-1))? 1:0;
            break;
        case CmpGT:
            getDestInt(IP-1) = (getDestInt(IP-1)>getSrcInt(IP-1))? 1:0;
            break;
        case CmpGE:
            getDestInt(IP-1) = (getDestInt(IP-1)>=getSrcInt(IP-1))? 1:0;
            break;
        default:
            break;
        }
    }
    else {
        switch(memBank[IP-1].code) {
        case CmpEQ: 
            getDestInt(IP-1) = (((isDestInt)? getDestInt(IP-1):getDestReal(IP-1))==((isSrcInt)? getSrcInt(IP-1):getSrcReal(IP-1)))? 1:0;
            break;
        case CmpNE:
            getDestInt(IP-1) = (((isDestInt)? getDestInt(IP-1):getDestReal(IP-1))!=((isSrcInt)? getSrcInt(IP-1):getSrcReal(IP-1)))? 1:0;
            break;
        case CmpLT:
            getDestInt(IP-1) = (((isDestInt)? getDestInt(IP-1):getDestReal(IP-1))<((isSrcInt)? getSrcInt(IP-1):getSrcReal(IP-1)))? 1:0;
            break;
        case CmpLE:
            getDestInt(IP-1) = (((isDestInt)? getDestInt(IP-1):getDestReal(IP-1))<=((isSrcInt)? getSrcInt(IP-1):getSrcReal(IP-1)))? 1:0;
            break;
        case CmpGT:
            getDestInt(IP-1) = (((isDestInt)? getDestInt(IP-1):getDestReal(IP-1))>((isSrcInt)? getSrcInt(IP-1):getSrcReal(IP-1)))? 1:0;
            break;
        case CmpGE:
            getDestInt(IP-1) = (((isDestInt)? getDestInt(IP-1):getDestReal(IP-1))>=((isSrcInt)? getSrcInt(IP-1):getSrcReal(IP-1)))? 1:0;
            break;
        default:
            break;
        }
    }
}
void VM::MoveData() {
    getDestType(IP-1) = getSrcType(IP-1);
    if(getDestType(IP-1) == is_real) {
        getDestReal(IP-1) = getSrcReal(IP-1);
    }
    else {
        getDestInt(IP-1) = getSrcInt(IP-1);
    }
}
void VM::StoreData() {
    //*dest = src
    staticMem[getDestInt(IP-1)].type = getSrcType(IP-1);
    if(getSrcReal(IP-1) == is_real) {
        staticMem[getDestInt(IP-1)].val.rval = getSrcReal(IP-1);
    }
    else {
        staticMem[getDestInt(IP-1)].val.ival = getSrcInt(IP-1);
    }
}
void VM::LoadData() {
    //dest = *src
    getDestType(IP-1) = staticMem[getSrcInt(IP-1)].type;
    if(getSrcReal(IP-1) == is_real) {
        getDestReal(IP-1) = staticMem[getSrcInt(IP-1)].val.rval;
    }
    else {
        getDestInt(IP-1) = staticMem[getSrcInt(IP-1)].val.ival;
    }
}
void VM::OutputData() {
    switch(getSrcType(IP-1)) {
    case is_int:
        cout<<getSrcInt(IP-1);
        break;
    case is_real:
        cout<<getSrcReal(IP-1);
        break;
    case is_bool:
        if(getSrcInt(IP-1) == 0) {
            cout<<"false";
        }
        else {
            cout<<"true";
        }
        break;
    case is_addr:
        cout<<getSrcInt(IP-1)<<"A";
    default:
        break;
    }
}
void VM::InputData() {
    string inputStr;
    cin>>inputStr;
    smatch temp;
    
    if(inputStr == "true" || inputStr == "false") {
        getDestType(IP-1) = is_bool;
        getDestInt(IP-1) = (inputStr == "true")? 1:0;
    }
    else if(regex_search(inputStr, temp, addr)) {
        getDestType(IP-1) = is_addr;
        getDestInt(IP-1) = stoi(temp.str(1), NULL);
    }
    else if(regex_search(inputStr, temp, real)) {
        getDestType(IP-1) = is_real;
        getDestReal(IP-1) = stod(temp.str(1), NULL);
    }
    else if(regex_search(inputStr, temp, integer)) {
        getDestType(IP-1) = is_int;
        getDestInt(IP-1) = stoi(temp.str(1), NULL);
    }
    else {
        throw InvalidInput(IP-1);
    }
}
    
void VM::printCMD() {
    cout<<endl<<endl<<"int:"<<0<<" real:"<<1<<" bool:"<<2<<" addr:"<<3<<" reg:"<<4<<endl;
    for(int i=0; i<numCMD; i++) {
        cout<<"Code: "<<memBank[i].code<<" Dest: "<<memBank[i].reg<<" Src type: ";
        
        switch(memBank[i].src.type) {
        case 0:
            cout<<"int";
            break;
        case 1:
            cout<<"real";
            break;
        case 2:
            cout<<"bool";
            break;
        case 3:
            cout<<"address";
            break;
        case 4:
            cout<<"register";
            break;
        case 5:
            cout<<"non";
        default:
            break;
        }
        
        cout<<" -> "<<memBank[i].src.val.ival<<"  "<<memBank[i].src.val.rval<<endl;
    }
}
void VM::printReg() {
    for(int i=0; i<15;i++) {
        if(regis[i].type == is_non) continue;
        cout<<"R"<<i+1<<" -> "<<"type: ";
        switch(regis[i].type) {
        case 0:
            cout<<"int";
            cout<<"  value: "<<regis[i].val.ival<<endl;
            break;
        case 1:
            cout<<"real";
            cout<<"  value: "<<regis[i].val.rval<<endl;
            break;
        case 2:
            cout<<"bool";
            cout<<"  value: "<<regis[i].val.ival<<endl;
            break;
        case 3:
            cout<<"address";
            cout<<"  value: "<<regis[i].val.ival<<endl;
            break;
        case 5:
            cout<<"non";
            break;
        default:
            break;
        }
    }
    cout<<"------------------------"<<endl;
}
