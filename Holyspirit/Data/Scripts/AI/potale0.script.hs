main 
	if talk
	then
		if variable * 1 * 0 
		then
			speak * 38
			variable * 1 * 1
			speak_choice * 39 * 1
			speak_choice * 40 * 2
		end
		
		if speak_choice * 1
		then
			stop_speak
			teleportation_menu
		end
		
		if speak_choice * 2
		then
			stop_speak
			trade * 1
		end
	else
		variable * 1 * 0
	end
	
end 