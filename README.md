# IPP-duze-zadanie

Task:

This task is to implement operations on road maps.

Road map is a collection of cities connected by roads.

The city has information about its name, which is a non-empty inscription without signs
with codes from 0 to 31 or a semicolon and terminated with a zero sign.

A road section connects two cities with each other. There can be only one section of road between two cities.
The road section has information about the year of construction or the last renovation and about the length. Length
is a positive integer. Year of construction or last renovation is an integer where
positive values represent the years of our era, and negative values years before our era.
There was no year 0.

A route is a road, i.e. a series of road sections connecting two cities, without breaks,
self-cutting and loops. One road section may belong to many routes. Route
has information about its number, which is an integer between 1 and 999.
The length of the route is the sum of the length of road sections that the route crosses.

The program also has a text interface. Reads from standard input, writes results
to the standard output, and error information to the standard diagnostic output.

The program accepts the following commands:

- The command that creates a route:
route number;city name;length of road section;year of construction or last renovation;city name;length of road section;year of construction or last renovation;city name; ...;city name
This command creates a route with the given number and course. If some stretch of road
by the command does not exist, the program creates it. In the event that the road section already exists,
but its year of construction or last renovation is earlier than the year of the section given,
it modifies the year. Information about the error is printed when the given road section already exists,
but has a different length or later construction year or the last renovation year.
This command prints nothing to standard output.

- The command that creates the road section:
addRoad;city1;city2;length;builtYear
This command calls the addRoad function with the given parameters.
The command prints nothing to standard output.

- Command repairing the road section:
repairRoad;city1;city2;repairYear
This command calls the repairRoad function with the given parameters.
The command prints nothing to standard output.

- Command writing a description of the route:
getRouteDescription;routeId
This command calls the getRouteDescription function with the given parameters.
If the result of the function has a non-NULL value, it prints one line to the standard output
with the result of this function.

- The command that creates route with finding the shortest way to do this:
newRoute;routeId;city1;city2
This command calls the newRoute function with the given parameters.
The command prints nothing to standard output.

- Command that extends the route:
extendRoute;routeId;city
This command calls the extendRoute function with the given parameters.
The command prints nothing to standard output.

- Command that removes road section:
removeRoad;city1;city2
This command calls the removeRoad function with the given parameters.
The command prints nothing to standard output.

- Command that removes route:
removeRoute;routeId
This command calls the removeRoute function with the given parameters.
The command prints nothing to standard output.

The program skips blank lines and lines whose first character is '#'.

If the command is incorrect or its execution ended in error, it writes to standard
diagnostic output character error information:
ERROR n
where n is the line number in the input containing this command.
The lines are numbered from one.
