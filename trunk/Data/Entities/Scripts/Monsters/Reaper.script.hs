
main
	if variable * 9 * 0 
	then
		if alive
		then
			if see
			then
				if distance * 4
				then
				
					if miracle * 0
					then
						setState * 4
					else
						fight
						
						incrementVariable * 2 * time
						incrementVariable * 3 * time
						
						if variable_bigger * 2 * 10
						then
							setState * 5 * 0
							variable * 2 * 0
						end
						
						if variable_bigger * 3 * 20
						then
							setState * 6 * 0
							variable * 3 * 0
							variable * 4 * 1
						end
					end
				else
					if getState * 6
					then
					else
						useMiracle * 0
						if getState * 4
						then
						else
							setState * 4 * 0
							variable * 2 * 0
						end
					end
				end
			else
				randomDisplace
			end
		end
	end
end
