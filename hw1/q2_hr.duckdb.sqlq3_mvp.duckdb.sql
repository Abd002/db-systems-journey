select DISTINCT (p.nameFirst ||' (' || p.nameGiven || ') ' || p.nameLast) as name, max(a.HR) as max_hr_apperance
from appearances AS a 
join collegeplaying AS c ON a.playerID = c.playerID 
join schools as s on s.schoolID = c.schoolID and s.state = 'PA' 
join people as p on p.playerID = a.playerID  
GROUP by name  
order by max_hr_apperance  desc  
limit 10;
