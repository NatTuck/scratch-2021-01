

# Project Proposal Questions?


# Today: Passwords

 - Apps have user accounts.
 - We'd like our user accounts to correspond to
   actual, concrete people.
 - Authentication: We want to be able to assure that
   if Alice makes an account, then some other user
   - Mallory - can't control that account.
 - When a user logs in - creating a session - we
   want to authenticate that they're the same
   person who created the account.
 - Bad news: That's actually impossible.
 - But we can do some stuff that's kind of similar.

Authentication ideas:

 - Link to already authenticated account - e.g.
   Facebook, Google, GitHub.
   - Still not identifying a person.
   - But, we can transfer most of the authentication
     properties of the alice123 account on Github
     to the alice account on our app.
 - Link to an email address or phone number.     
   - This is the same as linking to any other
     account.
 - Use a cryptographic public key.
   - Still not a person.
   - But we can guarantee that only a person or
     machine with access to the appropriate private
     key can authenticate as the linked user.
   - Most secure, strongest guarantee.
   - Approximately nobody does it because it would
     require at least user training.
 - Use a password
   - Link cryptographic keys, but way worse. 

## Solution: Passwords

 - User select a password when they register.
 - We assume that anyone who knows the password
   is the person.
 - How long a password do we need?

### Threat 1: Online Attacks

Mallory guesses passwords and tries to log
in to the app as Alice with them.

 - Mallory might be able to type in a password
   every second.
 - Assuming the password has random digits,
   0-9, how long does it take Mallory to guess
   Alice's password?
   - 4 digit bank pin = 10^4 = 10,000
   - = 167 minutes ~ 3 hours
 - What if Mallory writes a program to guess
   1000 passwords per second?
   - 4 digit bank pin takes: 10 seconds
 - What if instead of digits 0-9, we allow
   lowercase letters a-z?
   - 4 letter password
   - 1000 guesses per second
   - 26^4 seconds = 456k / 45k seconds = 0.55 days
 - What if instead of lowercase letters, we
   allow letters, numbers, and 10 symbols,
   so 72 possible characters.
   - 4 letter password
   - 1000 guesses per second
   - 72^4 possibilities, /1000 = 2.7M seconds
   -    like a day
 - Correct # of seconds for:
   - 72 characters, 6 character password
     - 72^6 = 1.3B = 1.3M seconds = 15 days
   - 72 characters, 8 character password
     - 72^8 = 722T = ~1B seconds = 11k days => 32 years

Against online attacks, a random 8 character password
is probably fine.

 - We probably don't want to allow shorter passwords if
   we're trying to resist this sort of attack.
 - With an online attack we can restrict attempts.
   - Doesn't have to be too strict - limiting to
     60 tries an hour is fine.
   - Someone should tell the Khoury admins that
     login.ccs doesn't need to ban you on 5 bad
     password tries.

## Threat 2: Offline Attacks

Mallory gets access to your server, then tries
to get Alice's password so she can log in as
Alice later.

Plan A: Store the passwords in our database.

 - Now Mallory has all the passwords for our
   app and can log in as anyone.
 - People frequently reuse passwords, so Mallory
   probably has a bunch of bank passwords now
   too.
 - This is bad: Let's not store passwords.

Plan B: Store a cryptographic hash of each password
  in our DB.
 
  - Cryptographic hash like sha256
  - Now Mallory doesn't have passwords, she just has
    hashes.
  - She'll need to try guessing passwords and see if
    they match Alice's hash.
    - After each guess, she can check every hash.
    - This is basically the same problem as mining
      Bitcoin - a GPU can easily test a billion
      hashes per second.
    - She can guess a 6 character password in 1 second.
    - An 8 character random password might take
      772,000 seconds = 9 days
  - Cryptograhic hash functions are desgined to
    be fast, and that's a weakness here.

Plan C: Store a hash of each password, using a 
  password hashing function.
  
  - Password hashing functions, like argon2, are
    cryptograhic hashes that are intentionally
    designed to be slow.
  - This makes the time to check a password tunable. 
  - Maybe we can reduce the guess rate to 100 passwords
    per second.
  - The cost here is that verifying a password in
    normal operation now takes 300ms or something.
  - Maybe the 8 character password is ok... if it's
    really random.

Problem: What if Mallory pre-calculates the hashes
for a large dictionary?

  - This pre-calcuation only needs to happen once,
    and pepole can share these "rainbow tables"
    online.
  - A couple gigs of password hashes is a lot
    of possible passwords.

Good Plan: Password hashing function + salt

 - Hash the password + a random number (the salt)
 - Store the salt with the password hash.
 - If we use a 32-bit salt, then any rainbow
   table needs to be 4 billion times bigger.

## Good password practices

Required for assignments in this class:

 - Minimum length: 10-12 characters
 - Use password hashing function + salt.
 - Don't require password changes.

Good practice in general:
 
 - Ban common passwords based on a explicit list.
 - Check for online attacks and throttle
   login attempts after some large number of tries

## Password Guidelines

You'll commonly see websites with rules like:

 - Passwords must be at least 8 characters.
 - Passwords must contain:
   - An uppercase letter.
   - A lowercase letter.
   - A digit.
   - A symbol.
   - An arabic character
   - An emojii.
   - An obscure mathematical symbol.
 - You must change your password every 30 days.

Result: Users make their password exactly:

 - "P4ssword+17"
 - Where "17" is the number of times this password
   has been changed.
 - And they're going to put it on a post-it on their
   monitor.

NIST guidlines currently say:

 - Minimize password form requirements.
 - Don't require password changes unless a
   password leak has actually occurred.
 - Do require minimum password lengths.
 - Do check passwords against a dictionary
   of common passwords and reject specific
   very common password.
 
## Two-factor authentication

In addition to a password, we require something else
as well to verify a user.

Examples:

  - Send an SMS to a phone #.
    - This has allowed millions of dollars to
      be stolen, and isn't even slightly secure.
  - Send an email.
    - This is just authentication by email.
  - Use a code provided by an app on a smartphone.
  - Use a code from a hardware token.
    - This is kind of crytographic-key-ish.
    - Except easier to manage.
    - If mandatory, risks excluding some users.

Question: Is the password still adding security?


# LocalStorage

There's an object called localStorage that lets
us save stuff in the browser.

 - The stuff is tied to (protocol, host, port)
 - The stuff lasts "forever". 
   - We want an expiration mechanic.
   - You want to explicitly handle the bad
     data case.
 - We can store String values associated with
   String keys.
 
 



