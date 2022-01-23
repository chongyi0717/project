#include "init.h"

vector<string> split(const string& str, const string& delim);
bool compare(Job first,Job second);
bool comparecho(Choromosome first,Choromosome second);
int main()
{
    //----------------------preprocess----------------------------//
    srand(time(NULL));
    ifstream wipfile(WIP_FILE);
    vector<Job> job;
    int m=0;
    int arrival_time[JOB_NUMBER];
    while(wipfile.good()){
        string text;
        getline(wipfile,text,'\n');
        if(m!=0 && m<=JOB_NUMBER)
        {
            Job *job_buffer=new Job();
            vector<string>res1 = split(text, ",");
            vector<string> res3 = split(res1.at(0),"LOT");
            job_buffer->job_name=res3.at(1);
            job_buffer->recipe=res1.at(8);
            job_buffer->arrival_time=stoi(res1.at(5));
            vector<string> res2 = split(res1.at(9),"EQP");
            vector<int> res2int;
            res2.erase(res2.begin());
            for (int j = 0; j < res2.size(); j++)
            {
               res2int.push_back(stoi(res2.at(j)));
            }
            job_buffer->machine_can_select=res2int;
            //cout<<m<<": "<<job_buffer->machine_can_select.size()<<endl;
            job.push_back(*job_buffer);
        }
        m++;
    }
    wipfile.close();
    m=0;
    vector <EQP_RECIPE> EQP;
    ifstream eqpfile(EQP_RECIPE_FILE);
     while(eqpfile.good()){
        string text;
        getline(eqpfile,text,'\n');
        if(m!=0 && m<=JOB_NUMBER)
        {
            EQP_RECIPE *EQP_buff=new EQP_RECIPE();
            vector<string>res1 = split(text, ",");
            EQP_buff->EQP_ID=stoi(res1.at(0));
            EQP_buff->RECIPE=res1.at(1);
            EQP_buff->process_time=stoi(res1.at(2));
            EQP.push_back(*EQP_buff);
        }
        m++;
    }
    eqpfile.close();
    int setuptime[JOB_NUMBER][JOB_NUMBER]={0};
    ifstream setupfile(set_up_time_FILE);
    m=0;
    while(setupfile.good()){
        string text;
        getline(setupfile,text,'\n');
        if(m!=0 && m<=JOB_NUMBER)
        {
            vector<string>res1 = split(text, ",");
            for(int i=1;i<=JOB_NUMBER;i++)
            {
                setuptime[m-1][i-1]=stoi(res1.at(i));
            }
        }
        m++;
    }
    setupfile.close();
    /*for(int i=0;i<JOB_NUMBER;i++)
    {
        cout<<i<<":"<<endl;
        for(int j=0;j<JOB_NUMBER;j++)
            cout<<setuptime[i][j]<<" ";
            cout<<endl;
    }*/
    vector<Choromosome>CHO(CHOROMOSOME_NUMBER*2);
    for(m=0;m<CHOROMOSOME_NUMBER*2;m++)
    { 
        for(int j=0;j<JOB_NUMBER*2;j++)
        {
            float value=rand() / (RAND_MAX + 1.0);
            CHO.at(m).value.push_back(value);
        }
    }
    //----------------------------preprocess end--------------------------//
    /*for(int i=0;i<CHO.size();i++)
    {
        cout<<"--------"<<i<<"----------"<<endl;
        for(int j=0;j<CHO.at(i).value.size();j++)
        cout<<CHO.at(i).value.at(j)<<" ";
        cout<<endl;
    }*/
    //----------------------------loop start----------------------------------------//
    m=0;
    while(1)
    {
        //--------------------------------mating-----------------------------------//
        int c=0;
        int a=CHOROMOSOME_NUMBER;
        while(c<CHOROMOSOME_NUMBER*MATING_RATE/2)
        {  
            int start=rand()%(JOB_NUMBER*2);
            int end=rand()%(JOB_NUMBER*2);
            int choro1=rand()%CHOROMOSOME_NUMBER;
            int choro2=rand()%CHOROMOSOME_NUMBER;
            if(start>=end || choro1==choro2)
                continue;
            //printf("%d %d %d %d\n",start+1,end+1,choro1+1,choro2+1);
            for(int j=0;j<JOB_NUMBER*2;j++)
            {
                CHO.at(a).value.at(j)=CHO.at(choro1).value.at(j);
                CHO.at(a+1).value.at(j)=CHO.at(choro2).value.at(j);
            }
            for(int j=start;j<=end;j++)
            {
                CHO.at(a).value.at(j)=CHO.at(choro2).value.at(j);
                CHO.at(a+1).value.at(j)=CHO.at(choro1).value.at(j);
            }
            c++;
            a+=2;
            
        }
        //----------------------------------------------mutation---------------------------------------//
        c=0;
        while(c<CHOROMOSOME_NUMBER*MUTATING_RATE)
        {  
            int choro=rand()%CHOROMOSOME_NUMBER;
            int rnd;
            
            for(int i=0;i<JOB_NUMBER*2;i++)
                CHO.at(a).value.at(i)=CHO.at(choro).value.at(i);
            for(int i=0;i<JOB_NUMBER*2*MUTATING_RATE_FOR_ONECHO;i++)
            {
                rnd=rand()%(JOB_NUMBER*2);
                CHO.at(a).value.at(rnd)=rand() / (RAND_MAX + 1.0);
            }
            
            c++;
            a++;
        }
        /*for(int i=0;i<CHOROMOSOME_NUMBER*2;i++)
        {
           cout<<"CHOROMOSOME"<<i+1<<" ";
            for(int j=0;j<JOB_NUMBER*2;j++)
            {
                //printf(" %.2f",CHO[i].value[j]);
                cout<<CHO.at(i).value.at(j);
            }
            cout<<endl;
            //printf("\t%d\n",CHO[i].makespan);         
        }*/
        try{
        for(int n=0;n<CHO.size();n++)
        {
            vector<Machine>machine(MACHINE_NUMBER);
            //cout<<"---------Choromosome"<<n<<":----------"<<endl;
            for(int i=0;i<JOB_NUMBER;i++)
            {
                job.at(i).select=CHO.at(n).value.at(i);
            }
            for(int i=JOB_NUMBER;i<JOB_NUMBER*2;i++)
            {
                job.at(i-JOB_NUMBER).sort=CHO.at(n).value.at(i);
            }
            /*for(int i=0;i<job.size();i++)
            {
                cout<<n<<" "<<i<<" "<<job.at(i).select<<" "<<job.at(i).sort<<endl;
            }*/
            //---------------------------------------------------select-----------------------------------------------//
            for(int i=0;i<job.size();i++)
            {
                float buffer_of_number_for_add=(float)1/job.at(i).machine_can_select.size();
                float buffer_for_adding_to_one=0.00;
                for(int j=0;j<=job.at(i).machine_can_select.size();j++)
                {
                    //cout<<buffer_for_adding_to_one<<" "<<job.at(i).select<<endl;;
                    if(job.at(i).select>=buffer_for_adding_to_one-buffer_of_number_for_add && job.at(i).select<=buffer_for_adding_to_one)
                    {
                        //cout<<job.at(i).machine_can_select.at(j-1)<<" ";
                        if(j==0)
                            job.at(i).machine_selected=job.at(i).machine_can_select.at(j);
                        else
                            job.at(i).machine_selected=job.at(i).machine_can_select.at(j-1);
                        //try{job.at(i).machine_selected=job.at(i).machine_can_select.at(j-1);}
                        //catch(exception &e){cout<<"exception! "<<j<<endl;return 1;}
                        for(int k=0;k<EQP.size();k++)
                        {
                            if(job.at(i).machine_selected==EQP.at(k).EQP_ID && job.at(i).recipe==EQP.at(k).RECIPE)
                            {
                                job.at(i).process_time=EQP.at(k).process_time;
                                break;
                                //cout<<job.at(i).machine_selected<<" "<<job.at(i).process_time<<endl;
                            }
                        
                        }
                        machine.at(job.at(i).machine_selected-1).job_in_machine.push_back(job.at(i));
                        //cout<<buffer_for_adding_to_one<<" "<<job.at(i).select<<" "<<job.at(i).machine_selected<<endl;
                        break;
                    }
                    buffer_for_adding_to_one+=buffer_of_number_for_add;
                }   
            }
            /*int sum=0;
            for(int i=0;i<machine.size();i++)
            {
                sum+=machine.at(i).job_in_machine.size();    
                cout<<"Machine"<<i<<": ";
                for(int j=0;j<machine.at(i).job_in_machine.size();j++)
                    cout<<machine.at(i).job_in_machine.at(j)<<" ";
                cout<<endl;
            }
            cout<<"Machine size in Choromosome"<<n<<":"<<sum<<endl;*/
            //-------------------select end------------------------------------------------------------------------//
             //-------------------------------------sort-----------------------------------------------------------//
            /*for(int i=0;i<machine.size();i++)
            {
                cout<<"----"<<i<<"--------"<<endl;
                for(int j=0;j<machine.at(i).job_in_machine.size()-1;j++)
                {
                    cout<<machine.at(i).job_in_machine.at(j).job_name<<" "<<machine.at(i).job_in_machine.at(j).sort<<" "<<machine.at(i).job_in_machine.at(j).process_time<<endl;
                }
                cout<<endl;
            }*/
            for(int i=0;i<machine.size();i++)
            {
                sort(machine.at(i).job_in_machine.begin(),machine.at(i).job_in_machine.end(),compare);
            }
            /*for(int i=0;i<machine.size();i++)
            {
                cout<<"----"<<i<<"--------"<<endl;
                for(int j=0;j<machine.at(i).job_in_machine.size()-1;j++)
                {
                    cout<<machine.at(i).job_in_machine.at(j).job_name<<" "<<machine.at(i).job_in_machine.at(j).sort<<" "<<machine.at(i).job_in_machine.at(j).process_time<<endl;
                }
                cout<<endl;
            }*/
            
            //-------------------sort end-----------------------------------//
            //-----------------------add setup time----------------------//
            for(int i=0;i<machine.size();i++)
            {
                //cout<<"----"<<i<<"--------"<<endl;
                if(machine.at(i).job_in_machine.size()<=1)
                    continue;
                machine.at(i).makespan=0;
                for(int j=0;j<machine.at(i).job_in_machine.size()-1;j++)
                {
                    //cout<<machine.at(i).job_in_machine.at(j)<<" "<<machine.at(i).process_time.at(j)<<" ";
                    
                    if(machine.at(i).job_in_machine.at(j).arrival_time<=machine.at(i).makespan)
                    {
                        machine.at(i).job_in_machine.at(j).process_time+=setuptime[stoi(machine.at(i).job_in_machine.at(j).job_name)-1][stoi(machine.at(i).job_in_machine.at(j+1).job_name)-1];
                    }
                    else
                    {
                        machine.at(i).job_in_machine.at(j).process_time+=(machine.at(i).job_in_machine.at(j).arrival_time-machine.at(i).makespan);
                    }
                    machine.at(i).makespan+=machine.at(i).job_in_machine.at(j).process_time;
                }
                 machine.at(i).makespan+=machine.at(i).job_in_machine.at(machine.at(i).job_in_machine.size()-1).process_time;
                //cout<<endl;
            }
           
            CHO.at(n).machine=machine;
            /*for(int i=0;i<machine.size();i++)
            {
                cout<<"----"<<i<<"--------"<<endl;
                for(int j=0;j<machine.at(i).job_in_machine.size();j++)
                {
                    cout<<machine.at(i).job_in_machine.at(j)<<" "<<machine.at(i).sort.at(j)<<" "<<machine.at(i).process_time.at(j)<<endl;
                }
                cout<<endl;
            }*/
            //----------------------------------------add set up time end--------------------------------//
            
        }//cho
        }
        catch(exception& e)
        {
            cout<<"vector is out of range"<<endl;
            return 1;
        }//try
        //---------------------sort choromosome------------------//
        for(int i=0;i<CHO.size();i++)
        {
            int max=0;
            for(int j=0;j<CHO.at(i).machine.size();j++)
            {
                if(max<CHO.at(i).machine.at(j).makespan)
                    max=CHO.at(i).machine.at(j).makespan;
            }
            CHO.at(i).makespan=max;  
        }
       // for(int i=0;i<CHO.size();i++)
            //cout<<"Choromosome "<<i+1<<": "<<CHO.at(i).makespan<<endl;
        sort(CHO.begin(),CHO.end(),comparecho);
        //cout<<CHO.at(CHOROMOSOME_NUMBER*2-1).makespan-CHO.at(0).makespan<<endl;
        //for(int i=0;i<CHO.size();i++)
            //cout<<"Choromosome "<<i+1<<": "<<CHO.at(i).makespan<<endl;
        //--------------------------sort choromosome end---------------------//
        //----------------elite-----------------//
        c=0;
        while(c<CHOROMOSOME_NUMBER*MUTATING_RATE)
        {
            CHO.at(c).roulette=0.0;
            c++;
        }
        //---------------rotary-----------------//
        while(c<CHOROMOSOME_NUMBER)
        {
            float sum=0.0,rnd=0.0;
            int j=1;
            for(int i=c;i<(CHOROMOSOME_NUMBER*2)-1;i++)
            {
                CHO.at(i).roulette=j;
                sum+=CHO.at(i).roulette;
                if(CHO.at(i).makespan!=CHO.at(i+1).makespan)
                    j++;
                //cout<<c<<" "<<CHO.at(i).roulette<<" "<<CHO.at(i).makespan<<" "<<sum<<endl;
            }
            for(int i=c;i<CHOROMOSOME_NUMBER*2;i++)
                CHO.at(i).roulette/=sum;
            rnd=rand() / (RAND_MAX + 1.0);
            sum=0.0;
            for(int i=c;i<CHOROMOSOME_NUMBER*2;i++)
            {
                sum+=CHO.at(i).roulette;
                if(rnd<sum)
                {
                    //cout<<rnd<<" "<<sum<<" "<<i<<endl;
                    for(int j=0;j<JOB_NUMBER*2;j++)
                    {
                        CHO.at(c).value.at(j)=CHO.at(i).value.at(j);
                    }
                    break;
                }
            }
            
            c++;
        }
        //-----------------------elite and rotary end-------------------------//
        
        //cout<<"Iterative "<<m+1<<"\t:Smallest Makespan = "<<CHO.at(0).makespan<<endl;
        //-----------------------------loop end--------------------------------------//
        if(CHO.at(0).makespan<1000)
            break;
        
    }//iterate
        ifstream smallestin("smallest_makespan.txt");
        string text;
        
        getline(smallestin,text,'\n');
        if(stoi(text)>CHO.at(0).makespan)
        {
            ofstream smallestout("smallest_makespan.txt");
            smallestout<<CHO.at(0).makespan<<endl<<"Choromosome Number = "<<CHOROMOSOME_NUMBER<<endl<<"Iterative Number = "<<m<<endl;
            for(int i=0;i<CHO.at(0).machine.size();i++)
            {
                smallestout<<"Machine"<<i+1<<": ";
                for(int j=0;j<CHO.at(0).machine.at(i).job_in_machine.size();j++)
                {
                    smallestout<<CHO.at(0).machine.at(i).job_in_machine.at(j).job_name<<"("<<CHO.at(0).machine.at(i).job_in_machine.at(j).process_time<<") ";
                }
                smallestout<<"\tMakespan: "<<CHO.at(0).machine.at(i).makespan<<endl;
            }
            smallestout<<"Choromosome: ";
            for(int i=0;i<CHO.at(0).value.size();i++)
                smallestout<<CHO.at(0).value.at(i)<<" ";
        }
    for(int i=0;i<CHO.at(0).machine.size();i++)
    {
        cout<<"Machine"<<i+1<<": ";
        for(int j=0;j<CHO.at(0).machine.at(i).job_in_machine.size();j++)
        {
            cout<<CHO.at(0).machine.at(i).job_in_machine.at(j).job_name<<"("<<CHO.at(0).machine.at(i).job_in_machine.at(j).process_time<<") ";
        }
        cout<<"\tMakespan: "<<CHO.at(0).machine.at(i).makespan<<endl;
    }
    cout<<"Smallest Makespan = "<<CHO.at(0).makespan<<endl;
    
    return 0;
}

string reverse_one_word(string str) {
	for(int i = 0; i < str.length()/2; i ++) {
		char tmp;
		tmp = str[i];
		str[i] = str[ str.length() - i - 1 ];
		str[ str.length() - i - 1 ] = tmp;
	}
	return str;
}
 
vector<string>  split(const string& str,const string& delim) { 
	vector<string> res;
	if("" == str) return  res;
	
	string strs = str + delim; 
	size_t pos;
	size_t size = strs.size();
 
	for (int i = 0; i < size; ++i) {
		pos = strs.find(delim, i); 
		if( pos < size) { 
			string s = strs.substr(i, pos - i);
			res.push_back(s);
			i = pos + delim.size() - 1;
		}
	}
	return res;	
}

bool compare(Job first,Job second)
{
    return first.sort>second.sort;
}

bool comparecho(Choromosome first,Choromosome second)
{
    return first.makespan<second.makespan;
}