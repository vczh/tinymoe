using System;
using System.Collections.Generic;
using TinymoeDotNet;

namespace TinymoeProgramNamespace
{
	public class TinymoeProgram : TinymoeOperations
	{
		public void standard_library__trap__expression_with_fall_back__expression_restored_internal(TinymoeObject state, TinymoeObject value, TinymoeObject fall_back, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject the_current_trap = null;
			the_current_trap = new standard_library__continuation_trap().SetFields(new TinymoeObject[] {continuation, state.GetField("trap")}).FinishConstruction();
			TinymoeObject _state_0 = null;
			_state_0 = state;
			standard_library__restored_trap__expression_with_fall_back__expression_and_counter__primitive(
				_state_0,
				the_current_trap,
				fall_back.GetField("trap"),
				fall_back.GetField("fall_back_counter"),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _result_3 = __args__[1];
					TinymoeObject new_trap = null;
					new_trap = _result_3;
					state.SetField("trap", new_trap);
					Invoke(value, new TinymoeObject[] {
						_state_2,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _result_5 = __args___x2[1];
							_the_result = _result_5;
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						});
				})
				);
		}

		public void standard_library__trap__expression_with_fall_back__expression_restored(TinymoeObject state, TinymoeObject value, TinymoeObject fall_back, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression_with_fall_back__expression_restored_internal(
				state,
				value,
				fall_back,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("trap", state.GetField("trap").GetField("previous_trap"));
					standard_library__set_fall_back_counter_of__expression(
						_state_0,
						state,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__fall_into_the_previous_trap(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject the_current_trap = null;
			the_current_trap = state.GetField("trap").GetField("continuation");
			TinymoeObject _state_0 = null;
			_state_0 = state;
			Invoke(the_current_trap, new TinymoeObject[] {
				_state_0,
				null
				});
		}

		public void standard_library__break(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__breaking_repeating,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__continue(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__continuing_repeating,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__raise__expression(TinymoeObject state, TinymoeObject exception, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__raising_exception,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("argument", exception);
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__exit_program(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__exiting_program,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__exit_block__expression(TinymoeObject state, TinymoeObject handle, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				standard_library__exiting_block,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("argument", handle);
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void standard_library__repeat(TinymoeObject state, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression(
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _continuation_1 = __args__[1];
					Invoke(body, new TinymoeObject[] {
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(_continuation_1, new TinymoeObject[] {
								_state_0,
								_result_3
								});
						})
						});
				}),
				new TinymoeFunction(__args___x3 => 
				{
					TinymoeObject _state_4 = __args___x3[0];
					TinymoeObject _result_5 = __args___x3[1];
					TinymoeObject _select_continuation_8 = null;
					TinymoeObject _select_value_9 = null;
					_select_continuation_8 = new TinymoeFunction(__args___x4 => 
					{
						TinymoeObject _state_6 = __args___x4[0];
						TinymoeObject _result_7 = __args___x4[1];
						Invoke(_continuation, new TinymoeObject[] {
							state,
							_the_result
							});
					});
					_select_value_9 = state.GetField("flag");
					if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, standard_library__breaking_repeating))).Value)
					{
						standard_library__reset_continuation_state__expression_to__expression(
							_state_4,
							state,
							null,
							_select_continuation_8
							);
					}
					else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, standard_library__continuing_repeating))).Value)
					{
						standard_library__reset_continuation_state__expression_to__expression(
							_state_4,
							state,
							null,
							new TinymoeFunction(__args___x5 => 
							{
								TinymoeObject _state_12 = __args___x5[0];
								TinymoeObject _result_13 = __args___x5[1];
								standard_library__repeat(
									_state_12,
									body,
									new TinymoeFunction(__args___x6 => 
									{
										TinymoeObject _state_14 = __args___x6[0];
										TinymoeObject _result_15 = __args___x6[1];
										standard_library__call__expression(
											_state_14,
											_result_15,
											_select_continuation_8
											);
									})
									);
							})
							);
					}
					else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, null))).Value)
					{
						standard_library__repeat(
							_state_4,
							body,
							new TinymoeFunction(__args___x7 => 
							{
								TinymoeObject _state_18 = __args___x7[0];
								TinymoeObject _result_19 = __args___x7[1];
								standard_library__call__expression(
									_state_18,
									_result_19,
									_select_continuation_8
									);
							})
							);
					}
					else
					{
						standard_library__fall_into_the_previous_trap(
							_state_4,
							_select_continuation_8
							);
					}
				})
				);
		}

		public void standard_library__repeat_while__expression(TinymoeObject _state, TinymoeObject body, TinymoeObject condition, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__repeat(
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					Invoke(condition, new TinymoeObject[] {
						_state_2,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _result_5 = __args___x2[1];
							standard_library__if__expression(
								_state_4,
								new TinymoeFunction(__args___x3 => standard_library__break(__args___x3[0], __args___x3[1])),
								Not(_result_5),
								new TinymoeFunction(__args___x4 => 
								{
									TinymoeObject _state_6 = __args___x4[0];
									TinymoeObject _result_7 = __args___x4[1];
									Invoke(body, new TinymoeObject[] {
										_state_6,
										_continuation_3
										});
								})
								);
						})
						});
				}),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_0 = __args___x5[0];
					TinymoeObject _result_1 = __args___x5[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__repeat_with__argument_from__expression_to__expression(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject lower_bound, TinymoeObject upper_bound, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject the_current_number = null;
			the_current_number = Sub(lower_bound, new TinymoeInteger(1));
			TinymoeObject _state_0 = null;
			_state_0 = _state;
			standard_library__repeat_while__expression(
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_6 = __args__[0];
					TinymoeObject _continuation_7 = __args__[1];
					standard_library__add__expression_to_assignable(
						_state_6,
						new TinymoeInteger(1),
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_8 = __args___x2[0];
							TinymoeObject _continuation_9 = __args___x2[1];
							Invoke(_continuation_9, new TinymoeObject[] {
								_state_8,
								the_current_number
								});
						}),
						new TinymoeFunction(__args___x3 => 
						{
							TinymoeObject _state_10 = __args___x3[0];
							TinymoeObject _input_11 = __args___x3[1];
							TinymoeObject _continuation_12 = __args___x3[2];
							the_current_number = _input_11;
							Invoke(_continuation_12, new TinymoeObject[] {
								_state_10,
								null
								});
						}),
						new TinymoeFunction(__args___x4 => 
						{
							TinymoeObject _state_13 = __args___x4[0];
							TinymoeObject _result_14 = __args___x4[1];
							Invoke(deal_with__expression, new TinymoeObject[] {
								_state_13,
								the_current_number,
								_continuation_7
								});
						})
						);
				}),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_2 = __args___x5[0];
					TinymoeObject _continuation_3 = __args___x5[1];
					Invoke(_continuation_3, new TinymoeObject[] {
						_state_2,
						LT(the_current_number, upper_bound)
						});
				}),
				new TinymoeFunction(__args___x6 => 
				{
					TinymoeObject _state_4 = __args___x6[0];
					TinymoeObject _result_5 = __args___x6[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__repeat_with__argument_in__expression(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			Invoke(items.GetField("$dispatch<>standard_library::repeat_with_$argument_in_$expression"), new TinymoeObject[] {
				_state,
				deal_with__expression,
				items,
				_continuation
				});
		}

		public void standard_library__repeat_with__argument_in__expression_array_(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__repeat_with__argument_from__expression_to__expression(
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject index = __args__[1];
					TinymoeObject _continuation_3 = __args__[2];
					Invoke(deal_with__expression, new TinymoeObject[] {
						_state_2,
						ArrayGet(items, index),
						_continuation_3
						});
				}),
				new TinymoeInteger(1),
				ArrayLength(items),
				new TinymoeFunction(__args___x2 => 
				{
					TinymoeObject _state_0 = __args___x2[0];
					TinymoeObject _result_1 = __args___x2[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__if__expression(TinymoeObject _state, TinymoeObject body, TinymoeObject condition, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = condition;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = _the_result;
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(true)))).Value)
			{
				Invoke(body, new TinymoeObject[] {
					_state,
					_select_continuation_2
					});
			}
			else
			{
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
		}

		public void standard_library__else_if__expression(TinymoeObject _state, TinymoeObject signal, TinymoeObject body, TinymoeObject condition, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = signal;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = _the_result;
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(false)))).Value)
			{
				Invoke(condition, new TinymoeObject[] {
					_state,
					new TinymoeFunction(__args___x2 => 
					{
						TinymoeObject _state_4 = __args___x2[0];
						TinymoeObject _result_5 = __args___x2[1];
						_the_result = _result_5;
						TinymoeObject _select_continuation_8 = null;
						TinymoeObject _select_value_9 = null;
						_select_continuation_8 = _select_continuation_2;
						_select_value_9 = _the_result;
						if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, new TinymoeBoolean(true)))).Value)
						{
							Invoke(body, new TinymoeObject[] {
								_state_4,
								_select_continuation_8
								});
						}
						else
						{
							Invoke(_select_continuation_8, new TinymoeObject[] {
								_state_4,
								null
								});
						}
					})
					});
			}
			else
			{
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
		}

		public void standard_library__else(TinymoeObject _state, TinymoeObject signal, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = signal;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = _the_result;
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(false)))).Value)
			{
				Invoke(body, new TinymoeObject[] {
					_state,
					_select_continuation_2
					});
			}
			else
			{
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
		}

		public void standard_library__try(TinymoeObject state, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression(
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _continuation_1 = __args__[1];
					Invoke(body, new TinymoeObject[] {
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(_continuation_1, new TinymoeObject[] {
								_state_0,
								_result_3
								});
						})
						});
				}),
				new TinymoeFunction(__args___x3 => 
				{
					TinymoeObject _state_4 = __args___x3[0];
					TinymoeObject _result_5 = __args___x3[1];
					TinymoeObject _select_continuation_8 = null;
					TinymoeObject _select_value_9 = null;
					_select_continuation_8 = new TinymoeFunction(__args___x4 => 
					{
						TinymoeObject _state_6 = __args___x4[0];
						TinymoeObject _result_7 = __args___x4[1];
						Invoke(_continuation, new TinymoeObject[] {
							state,
							_the_result
							});
					});
					_select_value_9 = state.GetField("flag");
					if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_9, standard_library__raising_exception))).Value)
					{
						_the_result = state.GetField("argument");
						standard_library__reset_continuation_state__expression_to__expression(
							_state_4,
							state,
							null,
							_select_continuation_8
							);
					}
					else
					{
						standard_library__fall_into_the_previous_trap(
							_state_4,
							_select_continuation_8
							);
					}
				})
				);
		}

		public void standard_library__else_try(TinymoeObject state, TinymoeObject signal, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__if__expression(
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					standard_library__trap__expression(
						_state_2,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _continuation_5 = __args___x2[1];
							Invoke(body, new TinymoeObject[] {
								_state_4,
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_6 = __args___x3[0];
									TinymoeObject _result_7 = __args___x3[1];
									Invoke(_continuation_5, new TinymoeObject[] {
										_state_4,
										_result_7
										});
								})
								});
						}),
						new TinymoeFunction(__args___x4 => 
						{
							TinymoeObject _state_8 = __args___x4[0];
							TinymoeObject _result_9 = __args___x4[1];
							TinymoeObject _select_continuation_12 = null;
							TinymoeObject _select_value_13 = null;
							_select_continuation_12 = _continuation_3;
							_select_value_13 = state.GetField("flag");
							if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_13, standard_library__raising_exception))).Value)
							{
								_the_result = state.GetField("argument");
								standard_library__reset_continuation_state__expression_to__expression(
									_state_8,
									state,
									null,
									_select_continuation_12
									);
							}
							else
							{
								standard_library__fall_into_the_previous_trap(
									_state_8,
									_select_continuation_12
									);
							}
						})
						);
				}),
				NE(signal, null),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_0 = __args___x5[0];
					TinymoeObject _result_1 = __args___x5[1];
					Invoke(_continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__catch__argument(TinymoeObject state, TinymoeObject signal, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__if__expression(
				state,
				body,
				NE(signal, null),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__finally(TinymoeObject state, TinymoeObject signal, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(body, new TinymoeObject[] {
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				});
		}

		public void standard_library__named_block__argument(TinymoeObject state, TinymoeObject body, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject handle = null;
			handle = new TinymoeObject().SetFields(new TinymoeObject[] {}).FinishConstruction();
			TinymoeObject _state_0 = null;
			_state_0 = state;
			standard_library__trap__expression(
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _continuation_3 = __args__[1];
					Invoke(body, new TinymoeObject[] {
						_state_2,
						handle,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_4 = __args___x2[0];
							TinymoeObject _result_5 = __args___x2[1];
							Invoke(_continuation_3, new TinymoeObject[] {
								_state_2,
								_result_5
								});
						})
						});
				}),
				new TinymoeFunction(__args___x3 => 
				{
					TinymoeObject _state_6 = __args___x3[0];
					TinymoeObject _result_7 = __args___x3[1];
					standard_library__if__expression(
						_state_6,
						new TinymoeFunction(__args___x4 => 
						{
							TinymoeObject _state_10 = __args___x4[0];
							TinymoeObject _continuation_11 = __args___x4[1];
							standard_library__reset_continuation_state__expression_to__expression(
								_state_10,
								state,
								null,
								_continuation_11
								);
						}),
						And(EQ(state.GetField("flag"), standard_library__exiting_block), EQ(state.GetField("argument"), handle)),
						new TinymoeFunction(__args___x5 => 
						{
							TinymoeObject _state_8 = __args___x5[0];
							TinymoeObject _result_9 = __args___x5[1];
							standard_library__else_if__expression(
								_state_8,
								_result_9,
								new TinymoeFunction(__args___x6 => standard_library__fall_into_the_previous_trap(__args___x6[0], __args___x6[1])),
								new TinymoeFunction(__args___x7 => 
								{
									TinymoeObject _state_14 = __args___x7[0];
									TinymoeObject _continuation_15 = __args___x7[1];
									Invoke(_continuation_15, new TinymoeObject[] {
										_state_14,
										NE(state.GetField("flag"), null)
										});
								}),
								new TinymoeFunction(__args___x8 => 
								{
									TinymoeObject _state_16 = __args___x8[0];
									TinymoeObject _result_17 = __args___x8[1];
									Invoke(_continuation, new TinymoeObject[] {
										state,
										_the_result
										});
								})
								);
						})
						);
				})
				);
		}

		public void standard_library__add__expression_to_assignable(TinymoeObject _state, TinymoeObject value, TinymoeObject _read_variable, TinymoeObject _write_variable, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(_read_variable, new TinymoeObject[] {
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_write_variable, new TinymoeObject[] {
						_state_0,
						Add(_result_1, value),
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				});
		}

		public void standard_library__substract__expression_from_assignable(TinymoeObject _state, TinymoeObject value, TinymoeObject _read_variable, TinymoeObject _write_variable, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(_read_variable, new TinymoeObject[] {
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_write_variable, new TinymoeObject[] {
						_state_0,
						Sub(_result_1, value),
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						});
				})
				});
		}

		public void standard_library__boolean_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("CastToBoolean")), new TinymoeObject[] {
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void standard_library__integer_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("CastToInteger")), new TinymoeObject[] {
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void standard_library__floating_point_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("CastToFloat")), new TinymoeObject[] {
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void standard_library__string_of__primitive(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("CastToString")), new TinymoeObject[] {
				_state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public readonly TinymoeObject standard_library__raising_exception = new TinymoeSymbol("standard_library__raising_exception");

		public readonly TinymoeObject standard_library__exiting_program = new TinymoeSymbol("standard_library__exiting_program");

		public readonly TinymoeObject standard_library__exiting_block = new TinymoeSymbol("standard_library__exiting_block");

		public class standard_library__continuation_trap : TinymoeObject
		{
			public standard_library__continuation_trap()
			{
				SetField("continuation", null);
				SetField("previous_trap", null);
			}
		}

		public class standard_library__continuation_fall_back_argument : TinymoeObject
		{
			public standard_library__continuation_fall_back_argument()
			{
				SetField("value", null);
				SetField("trap", null);
				SetField("fall_back_counter", null);
			}
		}

		public class standard_library__continuation_state : TinymoeObject
		{
			public standard_library__continuation_state()
			{
				SetField("flag", null);
				SetField("argument", null);
				SetField("continuation", null);
				SetField("trap", null);
			}
		}

		public void standard_library__reset_continuation_state__expression_to__expression(TinymoeObject _state, TinymoeObject state, TinymoeObject flag, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			state.SetField("flag", flag);
			state.SetField("argument", null);
			state.SetField("continuation", null);
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__enable_fall_back_to__expression_with__expression_and__expression(TinymoeObject _state, TinymoeObject state, TinymoeObject continuation, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			state.SetField("continuation", continuation);
			state.SetField("argument", new standard_library__continuation_fall_back_argument().SetFields(new TinymoeObject[] {value, state.GetField("trap")}).FinishConstruction());
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__fall_back_counter_between__expression_and__expression_from__primitive(TinymoeObject _state, TinymoeObject top_trap, TinymoeObject current_trap, TinymoeObject counter, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = NE(top_trap, current_trap);
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(true)))).Value)
			{
				standard_library__fall_back_counter_between__expression_and__expression_from__primitive(
					_state,
					top_trap.GetField("previous_trap"),
					current_trap,
					Add(counter, new TinymoeInteger(1)),
					new TinymoeFunction(__args___x2 => 
					{
						TinymoeObject _state_4 = __args___x2[0];
						TinymoeObject _result_5 = __args___x2[1];
						_the_result = _result_5;
						Invoke(_select_continuation_2, new TinymoeObject[] {
							_state_4,
							_result_5
							});
					})
					);
			}
			else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeBoolean(false)))).Value)
			{
				_the_result = counter;
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
			else
			{
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
		}

		public void standard_library__set_fall_back_counter_of__expression(TinymoeObject _state, TinymoeObject state, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject top_trap = null;
			top_trap = state.GetField("argument").GetField("trap");
			TinymoeObject _state_0 = null;
			_state_0 = _state;
			TinymoeObject current_trap = null;
			current_trap = state.GetField("trap");
			TinymoeObject _state_2 = null;
			_state_2 = _state_0;
			standard_library__fall_back_counter_between__expression_and__expression_from__primitive(
				_state_2,
				top_trap,
				current_trap,
				Negative(new TinymoeInteger(1)),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_4 = __args__[0];
					TinymoeObject _result_5 = __args__[1];
					TinymoeObject counter = null;
					counter = _result_5;
					state.GetField("argument").SetField("fall_back_counter", counter);
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void standard_library__restored_trap__expression_with_fall_back__expression_and_counter__primitive(TinymoeObject _state, TinymoeObject trap, TinymoeObject fall_back_trap, TinymoeObject counter, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject _select_continuation_2 = null;
			TinymoeObject _select_value_3 = null;
			_select_continuation_2 = new TinymoeFunction(__args__ => 
			{
				TinymoeObject _state_0 = __args__[0];
				TinymoeObject _result_1 = __args__[1];
				Invoke(_continuation, new TinymoeObject[] {
					_state,
					_the_result
					});
			});
			_select_value_3 = counter;
			if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_3, new TinymoeInteger(0)))).Value)
			{
				_the_result = trap;
				Invoke(_select_continuation_2, new TinymoeObject[] {
					_state,
					null
					});
			}
			else
			{
				standard_library__restored_trap__expression_with_fall_back__expression_and_counter__primitive(
					_state,
					trap,
					fall_back_trap.GetField("previous_trap"),
					Sub(counter, new TinymoeInteger(1)),
					new TinymoeFunction(__args___x2 => 
					{
						TinymoeObject _state_4 = __args___x2[0];
						TinymoeObject _result_5 = __args___x2[1];
						TinymoeObject previous_trap = null;
						previous_trap = _result_5;
						_the_result = new standard_library__continuation_trap().SetFields(new TinymoeObject[] {fall_back_trap.GetField("continuation"), previous_trap}).FinishConstruction();
						Invoke(_select_continuation_2, new TinymoeObject[] {
							_state_4,
							_result_5
							});
					})
					);
			}
		}

		public void standard_library__call__expression(TinymoeObject _state, TinymoeObject value, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void standard_library__trap__expression_internal(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject the_current_trap = null;
			the_current_trap = new standard_library__continuation_trap().SetFields(new TinymoeObject[] {continuation, state.GetField("trap")}).FinishConstruction();
			TinymoeObject _state_0 = null;
			_state_0 = state;
			state.SetField("trap", the_current_trap);
			Invoke(value, new TinymoeObject[] {
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _result_3 = __args__[1];
					_the_result = _result_3;
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				});
		}

		public void standard_library__trap__expression(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression_internal(
				state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("trap", state.GetField("trap").GetField("previous_trap"));
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void standard_library__trap__expression_with_fall_back_enabled_internal(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject the_current_trap = null;
			the_current_trap = new standard_library__continuation_trap().SetFields(new TinymoeObject[] {continuation, state.GetField("trap")}).FinishConstruction();
			TinymoeObject _state_0 = null;
			_state_0 = state;
			state.SetField("trap", the_current_trap);
			Invoke(value, new TinymoeObject[] {
				_state_0,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject _result_3 = __args__[1];
					_the_result = _result_3;
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				});
		}

		public void standard_library__trap__expression_with_fall_back_enabled(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__trap__expression_with_fall_back_enabled_internal(
				state,
				value,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					state.SetField("trap", state.GetField("trap").GetField("previous_trap"));
					standard_library__set_fall_back_counter_of__expression(
						_state_0,
						state,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public readonly TinymoeObject standard_library__breaking_repeating = new TinymoeSymbol("standard_library__breaking_repeating");

		public readonly TinymoeObject standard_library__continuing_repeating = new TinymoeSymbol("standard_library__continuing_repeating");

		public readonly TinymoeObject enumerable__yielding_return = new TinymoeSymbol("enumerable__yielding_return");

		public readonly TinymoeObject enumerable__yielding_break = new TinymoeSymbol("enumerable__yielding_break");

		public class enumerable__enumerable_collection : TinymoeObject
		{
			public enumerable__enumerable_collection()
			{
				SetField("body", null);
			}
		}

		public class enumerable__collection_enumerator : TinymoeObject
		{
			public enumerable__collection_enumerator()
			{
				SetField("current_yielding_result", null);
				SetField("body", null);
				SetField("continuation", null);
			}
		}

		public void enumerable__yield_return__expression(TinymoeObject state, TinymoeObject value, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				enumerable__yielding_return,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__enable_fall_back_to__expression_with__expression_and__expression(
						_state_0,
						state,
						continuation,
						value,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							standard_library__fall_into_the_previous_trap(
								_state_2,
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_4 = __args___x3[0];
									TinymoeObject _result_5 = __args___x3[1];
									Invoke(continuation, new TinymoeObject[] {
										state,
										_the_result
										});
								})
								);
						})
						);
				})
				);
		}

		public void enumerable__yield_break(TinymoeObject state, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__reset_continuation_state__expression_to__expression(
				state,
				state,
				enumerable__yielding_break,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					standard_library__fall_into_the_previous_trap(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							Invoke(continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void enumerable__new_enumerator_from__primitive(TinymoeObject _state, TinymoeObject enumerable, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = new enumerable__collection_enumerator().SetFields(new TinymoeObject[] {null, enumerable.GetField("body"), null}).FinishConstruction();
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void enumerable__move__expression_to_the_next(TinymoeObject state, TinymoeObject enumerator, TinymoeObject continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__named_block__argument(
				state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_2 = __args__[0];
					TinymoeObject moving_to_the_next = __args__[1];
					TinymoeObject _continuation_3 = __args__[2];
					standard_library__if__expression(
						_state_2,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_6 = __args___x2[0];
							TinymoeObject _continuation_7 = __args___x2[1];
							standard_library__trap__expression_with_fall_back_enabled(
								_state_6,
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_8 = __args___x3[0];
									TinymoeObject _continuation_9 = __args___x3[1];
									Invoke(enumerator.GetField("body"), new TinymoeObject[] {
										_state_8,
										new TinymoeFunction(__args___x4 => 
										{
											TinymoeObject _state_10 = __args___x4[0];
											TinymoeObject _result_11 = __args___x4[1];
											Invoke(_continuation_9, new TinymoeObject[] {
												_state_8,
												_result_11
												});
										})
										});
								}),
								new TinymoeFunction(__args___x5 => 
								{
									TinymoeObject _state_12 = __args___x5[0];
									TinymoeObject _result_13 = __args___x5[1];
									enumerator.SetField("body", null);
									Invoke(_continuation_7, new TinymoeObject[] {
										_state_12,
										_result_13
										});
								})
								);
						}),
						NE(enumerator.GetField("body"), null),
						new TinymoeFunction(__args___x6 => 
						{
							TinymoeObject _state_4 = __args___x6[0];
							TinymoeObject _result_5 = __args___x6[1];
							standard_library__else_if__expression(
								_state_4,
								_result_5,
								new TinymoeFunction(__args___x7 => 
								{
									TinymoeObject _state_18 = __args___x7[0];
									TinymoeObject _continuation_19 = __args___x7[1];
									standard_library__trap__expression_with_fall_back__expression_restored(
										_state_18,
										new TinymoeFunction(__args___x8 => 
										{
											TinymoeObject _state_20 = __args___x8[0];
											TinymoeObject _continuation_21 = __args___x8[1];
											Invoke(enumerator.GetField("continuation"), new TinymoeObject[] {
												_state_20,
												null
												});
										}),
										enumerator.GetField("current_yielding_result"),
										_continuation_19
										);
								}),
								new TinymoeFunction(__args___x9 => 
								{
									TinymoeObject _state_14 = __args___x9[0];
									TinymoeObject _continuation_15 = __args___x9[1];
									Invoke(_continuation_15, new TinymoeObject[] {
										_state_14,
										NE(enumerator.GetField("continuation"), null)
										});
								}),
								new TinymoeFunction(__args___x10 => 
								{
									TinymoeObject _state_16 = __args___x10[0];
									TinymoeObject _result_17 = __args___x10[1];
									standard_library__else(
										_state_16,
										_result_17,
										new TinymoeFunction(__args___x11 => 
										{
											TinymoeObject _state_28 = __args___x11[0];
											TinymoeObject _continuation_29 = __args___x11[1];
											standard_library__exit_block__expression(
												_state_28,
												moving_to_the_next,
												_continuation_29
												);
										}),
										new TinymoeFunction(__args___x12 => 
										{
											TinymoeObject _state_26 = __args___x12[0];
											TinymoeObject _result_27 = __args___x12[1];
											TinymoeObject _select_continuation_34 = null;
											TinymoeObject _select_value_35 = null;
											_select_continuation_34 = _continuation_3;
											_select_value_35 = state.GetField("flag");
											if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_35, enumerable__yielding_return))).Value)
											{
												enumerator.SetField("current_yielding_result", state.GetField("argument"));
												enumerator.SetField("continuation", state.GetField("continuation"));
												standard_library__reset_continuation_state__expression_to__expression(
													_state_26,
													state,
													null,
													_select_continuation_34
													);
											}
											else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_35, enumerable__yielding_break))).Value)
											{
												enumerator.SetField("current_yielding_result", null);
												enumerator.SetField("continuation", null);
												standard_library__reset_continuation_state__expression_to__expression(
													_state_26,
													state,
													null,
													_select_continuation_34
													);
											}
											else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_35, null))).Value)
											{
												enumerator.SetField("current_yielding_result", null);
												enumerator.SetField("continuation", null);
												Invoke(_select_continuation_34, new TinymoeObject[] {
													_state_26,
													null
													});
											}
											else
											{
												standard_library__fall_into_the_previous_trap(
													_state_26,
													_select_continuation_34
													);
											}
										})
										);
								})
								);
						})
						);
				}),
				new TinymoeFunction(__args___x13 => 
				{
					TinymoeObject _state_0 = __args___x13[0];
					TinymoeObject _result_1 = __args___x13[1];
					Invoke(continuation, new TinymoeObject[] {
						state,
						_the_result
						});
				})
				);
		}

		public void enumerable___primitive_reaches_the_end(TinymoeObject _state, TinymoeObject enumerator, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			_the_result = And(EQ(enumerator.GetField("body"), null), EQ(enumerator.GetField("continuation"), null));
			Invoke(_continuation, new TinymoeObject[] {
				_state,
				_the_result
				});
		}

		public void enumerable__current_value_of__primitive(TinymoeObject _state, TinymoeObject enumerator, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			enumerable___primitive_reaches_the_end(
				_state,
				enumerator,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					TinymoeObject _select_continuation_4 = null;
					TinymoeObject _select_value_5 = null;
					_select_continuation_4 = new TinymoeFunction(__args___x2 => 
					{
						TinymoeObject _state_2 = __args___x2[0];
						TinymoeObject _result_3 = __args___x2[1];
						Invoke(_continuation, new TinymoeObject[] {
							_state,
							_the_result
							});
					});
					_select_value_5 = _result_1;
					if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_5, new TinymoeBoolean(true)))).Value)
					{
						_the_result = null;
						Invoke(_select_continuation_4, new TinymoeObject[] {
							_state_0,
							null
							});
					}
					else if (((TinymoeBoolean)CastToBoolean(EQ(_select_value_5, new TinymoeBoolean(false)))).Value)
					{
						_the_result = enumerator.GetField("current_yielding_result").GetField("value");
						Invoke(_select_continuation_4, new TinymoeObject[] {
							_state_0,
							null
							});
					}
					else
					{
						Invoke(_select_continuation_4, new TinymoeObject[] {
							_state_0,
							null
							});
					}
				})
				);
		}

		public void enumerable__create_enumerable_to_assignable(TinymoeObject state, TinymoeObject body, TinymoeObject _read_receiver, TinymoeObject _write_receiver, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(_write_receiver, new TinymoeObject[] {
				state,
				new enumerable__enumerable_collection().SetFields(new TinymoeObject[] {}).FinishConstruction(),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_read_receiver, new TinymoeObject[] {
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_2 = __args___x2[0];
							TinymoeObject _result_3 = __args___x2[1];
							_result_3.SetField("body", body);
							Invoke(_continuation, new TinymoeObject[] {
								state,
								_the_result
								});
						})
						});
				})
				});
		}

		public void enumerable__repeat_with__argument_in__expression_enumerable_collection_(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			enumerable__new_enumerator_from__primitive(
				_state,
				items,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					TinymoeObject enumerator = null;
					enumerator = _result_1;
					standard_library__repeat_while__expression(
						_state_0,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_8 = __args___x2[0];
							TinymoeObject _continuation_9 = __args___x2[1];
							enumerable__move__expression_to_the_next(
								_state_8,
								enumerator,
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_10 = __args___x3[0];
									TinymoeObject _result_11 = __args___x3[1];
									enumerable__current_value_of__primitive(
										_state_10,
										enumerator,
										new TinymoeFunction(__args___x4 => 
										{
											TinymoeObject _state_12 = __args___x4[0];
											TinymoeObject _result_13 = __args___x4[1];
											Invoke(deal_with__expression, new TinymoeObject[] {
												_state_12,
												_result_13,
												_continuation_9
												});
										})
										);
								})
								);
						}),
						new TinymoeFunction(__args___x5 => 
						{
							TinymoeObject _state_2 = __args___x5[0];
							TinymoeObject _continuation_3 = __args___x5[1];
							enumerable___primitive_reaches_the_end(
								_state_2,
								enumerator,
								new TinymoeFunction(__args___x6 => 
								{
									TinymoeObject _state_4 = __args___x6[0];
									TinymoeObject _result_5 = __args___x6[1];
									Invoke(_continuation_3, new TinymoeObject[] {
										_state_2,
										Not(_result_5)
										});
								})
								);
						}),
						new TinymoeFunction(__args___x7 => 
						{
							TinymoeObject _state_6 = __args___x7[0];
							TinymoeObject _result_7 = __args___x7[1];
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void enumerable__print__expression(TinymoeObject _state, TinymoeObject message, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			Invoke(GetExternalFunction(new TinymoeString("Print")), new TinymoeObject[] {
				_state,
				message,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				});
		}

		public void enumerable__main(TinymoeObject _state, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			TinymoeObject numbers = null;
			enumerable__create_enumerable_to_assignable(
				_state,
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_7 = __args__[0];
					TinymoeObject _continuation_8 = __args__[1];
					standard_library__repeat_with__argument_from__expression_to__expression(
						_state_7,
						new TinymoeFunction(__args___x2 => 
						{
							TinymoeObject _state_11 = __args___x2[0];
							TinymoeObject i = __args___x2[1];
							TinymoeObject _continuation_12 = __args___x2[2];
							enumerable__print__expression(
								_state_11,
								Concat(new TinymoeString("Enumerating "), i),
								new TinymoeFunction(__args___x3 => 
								{
									TinymoeObject _state_13 = __args___x3[0];
									TinymoeObject _result_14 = __args___x3[1];
									enumerable__yield_return__expression(
										_state_13,
										i,
										_continuation_12
										);
								})
								);
						}),
						new TinymoeInteger(1),
						new TinymoeInteger(10),
						_continuation_8
						);
				}),
				new TinymoeFunction(__args___x4 => 
				{
					TinymoeObject _state_0 = __args___x4[0];
					TinymoeObject _continuation_1 = __args___x4[1];
					Invoke(_continuation_1, new TinymoeObject[] {
						_state_0,
						numbers
						});
				}),
				new TinymoeFunction(__args___x5 => 
				{
					TinymoeObject _state_2 = __args___x5[0];
					TinymoeObject _input_3 = __args___x5[1];
					TinymoeObject _continuation_4 = __args___x5[2];
					numbers = _input_3;
					Invoke(_continuation_4, new TinymoeObject[] {
						_state_2,
						null
						});
				}),
				new TinymoeFunction(__args___x6 => 
				{
					TinymoeObject _state_5 = __args___x6[0];
					TinymoeObject _result_6 = __args___x6[1];
					standard_library__repeat_with__argument_in__expression(
						_state_5,
						new TinymoeFunction(__args___x7 => 
						{
							TinymoeObject _state_19 = __args___x7[0];
							TinymoeObject number = __args___x7[1];
							TinymoeObject _continuation_20 = __args___x7[2];
							standard_library__if__expression(
								_state_19,
								new TinymoeFunction(__args___x8 => standard_library__break(__args___x8[0], __args___x8[1])),
								GE(number, new TinymoeInteger(5)),
								new TinymoeFunction(__args___x9 => 
								{
									TinymoeObject _state_21 = __args___x9[0];
									TinymoeObject _result_22 = __args___x9[1];
									enumerable__print__expression(
										_state_21,
										Concat(new TinymoeString("Printing "), number),
										_continuation_20
										);
								})
								);
						}),
						numbers,
						new TinymoeFunction(__args___x10 => 
						{
							TinymoeObject _state_17 = __args___x10[0];
							TinymoeObject _result_18 = __args___x10[1];
							Invoke(_continuation, new TinymoeObject[] {
								_state,
								_the_result
								});
						})
						);
				})
				);
		}

		public void _dispatch_fail__standard_library__repeat_with__argument_in__expression(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			TinymoeObject _the_result = null;
			standard_library__raise__expression(
				_state,
				new TinymoeString("Only enumerable object (like containers) can be iterated using the repeat-with-in statement."),
				new TinymoeFunction(__args__ => 
				{
					TinymoeObject _state_0 = __args__[0];
					TinymoeObject _result_1 = __args__[1];
					Invoke(_continuation, new TinymoeObject[] {
						_state,
						_the_result
						});
				})
				);
		}

		public void TinymoeArray___dispatch__standard_library__repeat_with__argument_in__expression(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			standard_library__repeat_with__argument_in__expression_array_(
				_state,
				deal_with__expression,
				items,
				_continuation
				);
		}

		public void enumerable__enumerable_collection___dispatch__standard_library__repeat_with__argument_in__expression_x2(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			enumerable__repeat_with__argument_in__expression_enumerable_collection_(
				_state,
				deal_with__expression,
				items,
				_continuation
				);
		}

		public void TinymoeObject___dispatch__standard_library__repeat_with__argument_in__expression_x3(TinymoeObject _state, TinymoeObject deal_with__expression, TinymoeObject items, TinymoeObject _continuation)
		{
			_dispatch_fail__standard_library__repeat_with__argument_in__expression(
				_state,
				deal_with__expression,
				items,
				_continuation
				);
		}

		public TinymoeProgram()
		{
			TinymoeObject.SetExtension(
				typeof(TinymoeArray),
				"$dispatch<>standard_library::repeat_with_$argument_in_$expression",
				new TinymoeFunction(__args__ => TinymoeArray___dispatch__standard_library__repeat_with__argument_in__expression(__args__[0], __args__[1], __args__[2], __args__[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(enumerable__enumerable_collection),
				"$dispatch<>standard_library::repeat_with_$argument_in_$expression",
				new TinymoeFunction(__args___x2 => enumerable__enumerable_collection___dispatch__standard_library__repeat_with__argument_in__expression_x2(__args___x2[0], __args___x2[1], __args___x2[2], __args___x2[3]))
				); 
			TinymoeObject.SetExtension(
				typeof(TinymoeObject),
				"$dispatch<>standard_library::repeat_with_$argument_in_$expression",
				new TinymoeFunction(__args___x3 => TinymoeObject___dispatch__standard_library__repeat_with__argument_in__expression_x3(__args___x3[0], __args___x3[1], __args___x3[2], __args___x3[3]))
				); 
		}

		static void Main(string[] args)
		{
			var program = new TinymoeProgram();
			var continuation = new TinymoeFunction((TinymoeObject[] arguments) =>
			{
			});
			var trap = new TinymoeProgram.standard_library__continuation_trap();
			trap.SetField("continuation", continuation);
			var state = new TinymoeProgram.standard_library__continuation_state();
			state.SetField("trap", trap);
			program.enumerable__main(state, continuation);
		}
	}
}
