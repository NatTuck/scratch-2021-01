
# HW 10 Questions?

Make sure that for every callback you get:

 - You're returning the correct value.
   - 0 on success (maybe > 0)
   - -errorno if there's an error 
 - Fill in any write parameters correctly.
   - For getattr, consider all the fields of
     the stat structure.
   - The mode is important - the examples
     in the starter code should be fine for
     files / directories on HW10.
 - "unlink" is the delete operation
 
# Today: Slides!

 - Virtual Machine Monitors
 - Authentication and Access Control

If someone has a file of cryptographically hashed
passwords, can they crack the passwords?

  - Brute force
    - Hashcat, John The Ripper
    - Dictionary attacks

Password Hashing Contest Winner: argon2

You run a program owned by root.
 
 - Does it run as you?  by default
 - Does it run as root? with setuid



