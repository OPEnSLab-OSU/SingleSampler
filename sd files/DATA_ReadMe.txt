Read me for formatting the Data.csv file !!

Data.csv should be opened in Google Sheets or MS Excel (or a similar program) 
for proper readability.

The first column of Data.csv is the time. It is in unformatted Unix Epoch Time, 
meaning it shows up as a big number representing the number of seconds since 1970.
To convert that big number into a human readable format, make a fourth column that
uses this formula:

=A1/86400+date(1970,1,1)

The second column is the current state of the machine - flushing, sampling, etc.

The third column is the load reading in grams.

The fourth column is the load reading in raw analog value.

The fifth column is time in milliseconds since the sampler was turned on.

