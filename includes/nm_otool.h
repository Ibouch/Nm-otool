
#ifndef NM_OTOOL_H
# define NM_OTOOL_H

struct		s_section
{
	char		*sectname;	/* name of this section */
	char		*segname;	/* segment this section goes in */
};

#endif




struct		ESPORT_DOFUS
{
	char	*team_name;
	char	*pseudo[4];
	char	*manager_name;
	bool	pass_les_poules;	/** This variable is always false **/
	bool	have_melon;			/** This variable is always true **/
};