#pragma once
/*
Implemented:
	I implemented essentially everything except BPT Remove. 
	My MMap presentation worked so I'm pretty glad on that.

Not Implemented:
	Exit command

Partially Implemented:
	Inserting works but the where conditions basically doesn't work.
	Where conditions work when you manually type it in, but it doesn't work when I try to implement it with the SQL class

Bugs:
	There are many bugs, one being the most prominent is that the parser properly parses the command however decides that
	it wants to print out every single record within the file. So that's one bug from SQL

	After further investigation, I don't think any of the where conditions where although it parses it correctly (meaning that it
	separates it into its given where condition positions)

	BPlusTree: Removing breadcrumbs is not good. I would say it's far from being good. 

	I think there are more bugs in my Parser/Table code that I may need to potentially correct, as I'm writing this, my assumption is that my
	records are being printed regardless of the where condition however when I manually do it with the parser with no involvment of the SQL
	class, it works so it's definitely an issue with my SQL class

	Another bug with the MMap presentation, it says to get ask to asker but I'm somehow getting aside as my output. My assumption is that
	it's just with the lower_bound condition so it's getting the value that's before "ask".

Notes:
	I do like this project but the problem is I didn't time it properly and did what you told us not to do, put it in the back
	burner. I didn't pick up the slack and unfortuantely that led me to not completely finish the project. 

	I also didn't do the .pro files as I didn't have enough time to download QT and complete the project, Both the SQL and the were unfortunately not completed as a result. 


	Also the SQL does not have an output file because it kept breaking, I will show it in the video that I will be recording.
*/