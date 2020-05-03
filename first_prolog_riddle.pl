print_solution([Head|Tail]) :-
	write(Head),
	nl,
	print_solution(Tail).


solution(S) :-
    S = [[Name1, Location1, Instrument1],
            [Name2, Location2, Instrument2],
            [Name3, Location3, Instrument3],
            [Name4, Location4, Instrument4],
            [Name5, Location5, Instrument5],
            [Name6, Location6, Instrument6]],
    
    
    Names = [zeynep,elif,nelin,ali,mehmet,yunus],
    permutation(Names,[Name1,Name2,Name3,Name4,Name5,Name6]),
    Locations = [kitchen,pantry,livingroom,diningroom,study,bathroom],
    permutation(Locations, [Location1,Location2,Location3,Location4,Location5,Location6]),
    Instruments = [hammer,knife,handsaw,chemical,gun,weldingmachine],
    permutation(Instruments, [Instrument1,Instrument2,Instrument3,Instrument4,Instrument5,Instrument6]),
    
    
	%•	Clue 1: In the kitchen, the man was not found with the knife, hammer, or welding machine. Which instrument, other than gun, was found in the kitchen?
	member([Name5,kitchen,Instrument5],S), (Name5 = ali ; Name5 = yunus ; Name5 = mehmet), (Instrument5 = handsaw ; Instrument5 = chemical),
    %•	Clue 2: Elif was either in the study or the bathroom; Zeynep was in the other. Which room was Elif found in?
   	member([elif,Location2,_],S), ((Location2 = study, Location1 = bathroom) ; (Location2 = bathroom , Location1 = study)),
    %•	Clue 3: The person with the hammer was not in the bathroom nor the dining room; also, this person was not Elif nor Ali. Then, who had the hammer in that room?
    member([Name6,Location6,hammer],S), Location6 \= diningroom, Location6 \=bathroom, Name6 \= elif, Name6 \= ali,
    %•	Clue 4: The welding machine was found in the study with a woman. Who was it?
    member([Name2,study,weldingmachine],S), (Name2 = zeynep ; Name2 = elif),
    %•	Clue 5: The instrument found in the living room was with either Mehmet or Ali. What instrument was that?
    member([Name6,livingroom,_],S), (Name6 = ali ; Name6 = mehmet),
    %•	Clue 6: The knife was not in the dining room. Where was it?
    member([_,diningroom,Instrument3],S), Instrument3 \= knife,
    %•	Clue 7: Zeynep was not with the instrument found in the study nor the pantry. What instrument was found with Zeynep?
    member([zeynep,Location1,_],S), (Location1 \= study , Location1 \= pantry),
    %•	Clue 8: Ali was found with the gun. In which room was the gun found?
    member([ali,_,gun],S),
    %the safety box was broken by a handsaw in the pantry
    member([_,pantry,handsaw],S),
    
	print_solution(S).
