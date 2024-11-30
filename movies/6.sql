SELECT AVG(rating) FROM ratings WHERE movie_id IN
(SElECT id FROM movies WHERE year = 2012);
