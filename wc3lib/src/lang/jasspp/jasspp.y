
%{
%}

//%token PP

%%


IncreaseLine				: NewLine
					{
						Vjassdoc::parser()->currentFile()->addLines(1);
					}
					;

DocComment				: LineDocComment IncreaseLine
					| BlockDocComment /* increase lines please */
					{
					}
					;

%% 
