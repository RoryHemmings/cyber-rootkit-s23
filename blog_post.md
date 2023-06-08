# Teaching CS Students the screts behind (ethical) Malware Engineering

**Disclaimer**
> I am not an expert in this topic, and this project is meerly my interpretation/attempt at rootkit development. If you have any suggestions for improvement, please suggest them on github issues in this [repo](https://docs.google.com/presentation/d/1U2sHxWP64e9oxL_PAg-F9cti82FOHWIan8AGZFJG8Ec/edit?usp=drive_link).

For ACM cyber this spring quarter, we decided to breanch off from our workshop/lecture based style of teaching to try something more practical. To achieve this we offered a "Malware Development Lab" and the "Secure OS development Lab" 2 "Cyber Labs" instead. Both were inteded to be low level cyber-security projects allowing people to get their hands dirty on the development side of cyber. This post details the development process, plans, and results of this quarters Malware Lab. 

## Lab Objectives

Overall we had three major objectives in teaching this lab.

1. Develop fully functional rootkits using low level c
2. Exploit a vulnerability to install our rootkit thus gaining persistent access 
3. Teach students how to apply the fundamentals behind operating systems and network programming to practical malware. 

However, we required little previous knowledge of these subject matters, and thus we had to fit all of these objectives into a total of 10 hours.

As it relates to the exact architecture of the rootkits, we took heavy inspiration from the one developed in this [post](https://h0mbre.github.io/Learn-C-By-Creating-A-Rootkit/). If you are intereseted in what we did, I **highly recommend** you read this post as it contains great in depth technical explanations of many of the features we added.

This included two basic features:
1. A trigger mechanism that calls a function upon ssh login attempts with a trigger username
2. A forked bind shell process started by the trigger mechanism

Additionally, I added some features similar to those detailed in the post including ls and netstat evasion.

In the end these base features gave us a malicious LD_PRELOAD rootkit that provided persistent shell access while hiding from ls and netstat.

## Cirriculum

Our approach was to spend the first three weeks having our attendees developing the base features (trigger, bind shell, exploit script), and then give them the remaining couple of weeks to add their own features. Finally at the end of the quarter we provided them a target server they were tasked to exploit, and then demo their features in front of an audience at our end of quarter symposium.

Our cirriculum followed this structure:

Week 3: OS concepts + System Call Hooking + Trigger Mechanism
Week 4: Networking + bind shell
Week 5: break for midterms
Week 6: buffer overflows + exploit script
Week 7: industry talk + overview of possible features
Week 8: work session
Week 9: demos

## System Call Hooking

The core of the rootkit revolves around one basic principle: System Call Hooking. The basic idea is that we want to insert some layer between the user and operating system without the users knowledge. That way we can manipulate the information going into the operating system, and filter the information going towards the user.

## Bind Shell

## Local testing

## Remote Testing

## Developed Features

## Future Developments

## Complications

setup was arduous
we spent more time 
probably better to have bigger groups
we could cut back on lectures even more

## Conclusion

## References

[Techincal Breakdown of similar LD_PRELOAD rootkit](https://h0mbre.github.io/Learn-C-By-Creating-A-Rootkit/).
[repo](https://github.com/RoryHemmings/cyber-rootkit-s23) 
[discord](https://discord.gg/j9dgf2q):

Slides:
* [week 1](https://docs.google.com/presentation/d/1EFRjWrZO-xo2kFhywVZy9M7Y5B-amZuXKzCrS-XNfLQ/edit?usp=drive_link)
* [week 2](https://docs.google.com/presentation/d/19CA9Lu7Mfybz7OL6jksWFF0hEyisUwTRJRgKflugenY/edit?usp=drive_link)
* [week 3](https://docs.google.com/presentation/d/1U2sHxWP64e9oxL_PAg-F9cti82FOHWIan8AGZFJG8Ec/edit?usp=drive_link)