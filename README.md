# Build a Database

A C program that builds a java database for student records

This program gets two input files, one with the student names and IDs, and the second with their marks, and it creates a java database that stores these records. The features implemented include sorting the records by name, or by ID using binary tree traversal, also it has binary search implemented to get a student by his name or by his ID.

Here is how the input is given (you can find both files with the code)

![Screenshot (406)](https://user-images.githubusercontent.com/76274266/119421239-eb4bd100-bd06-11eb-895f-0db158006af2.png)

Then we use the following commands to get output:
H (get help), ln (sort list by name), li (sort list by id), fi (find student using his ID) and fn (find student by his name)

Here are examples of what the program can do:

By executing the command ln, we can see that the output database is sorted by last name: 

![Screenshot (405)](https://user-images.githubusercontent.com/76274266/119421425-52698580-bd07-11eb-9cb4-8274c777a1f0.png)

By executing the command li, we can see that the output database is sorted by last name: 

![Screenshot (402)](https://user-images.githubusercontent.com/76274266/119421523-8cd32280-bd07-11eb-8a56-611e3143f326.png)

By executing the command fn/fi, we can see the record of the student we searched for:

![Screenshot (403)](https://user-images.githubusercontent.com/76274266/119421575-ae340e80-bd07-11eb-8514-f584d62462b6.png)

Finally, the code also supports wrong inputs as in the following:

![Screenshot (404)](https://user-images.githubusercontent.com/76274266/119421626-cdcb3700-bd07-11eb-8482-07d410c9d06c.png)
