using System;
using System.Collections.Generic;
using System.Linq;

namespace TinymoeDotNet
{
    public class TinymoeObject
    {
        private static UInt64 counter = 0;
        internal Dictionary<string, TinymoeObject> fields = new Dictionary<string, TinymoeObject>();
        private List<string> fieldNames = new List<string>();
        private bool finishedConstruction = false;
        private UInt64 id = counter++;

        public void SetField(string name, TinymoeObject value)
        {
            if (finishedConstruction)
            {
                if (!this.fieldNames.Contains(name))
                {
                    throw new ArgumentOutOfRangeException("name");
                }
                this.fields[name] = value;
            }
            else
            {
                this.fields[name] = value;
                this.fieldNames.Add(name);
            }
        }

        public TinymoeObject FinishConstruction()
        {
            this.finishedConstruction = true;
            return this;
        }

        public TinymoeObject SetFields(TinymoeObject[] values)
        {
            foreach (var x in this.fieldNames.Zip(values, Tuple.Create))
            {
                this.fields[x.Item1] = x.Item2;
            }
            return this;
        }

        public UInt64 Id
        {
            get
            {
                return this.id;
            }
        }

        public override string ToString()
        {
            return "(" + this.GetType().Name + ")" + id.ToString();
        }
    }

    public class TinymoeBoolean : TinymoeObject
    {
        public bool Value { get; private set; }

        public TinymoeBoolean(bool value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(boolean)" + Value.ToString();
        }
    }

    public class TinymoeInteger : TinymoeObject
    {
        public int Value { get; private set; }

        public TinymoeInteger(int value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(integer)" + Value.ToString();
        }
    }

    public class TinymoeFloat : TinymoeObject
    {
        public double Value { get; private set; }

        public TinymoeFloat(double value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(float)" + Value.ToString();
        }
    }

    public class TinymoeString : TinymoeObject
    {
        public string Value { get; private set; }

        public TinymoeString(string value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(string)" + Value.ToString();
        }
    }

    public class TinymoeSymbol : TinymoeObject
    {
        public string Value { get; private set; }

        public TinymoeSymbol(string value)
        {
            this.Value = value;
        }

        public override string ToString()
        {
            return "(symbol)" + Value.ToString();
        }
    }

    public class TinymoeArray : TinymoeObject
    {
        public TinymoeObject[] Elements { get; private set; }

        public TinymoeArray(int length)
        {
            this.Elements = new TinymoeObject[length];
        }

        public TinymoeArray(TinymoeObject[] values)
        {
            this.Elements = values.ToArray();
        }

        public override string ToString()
        {
            return "[" + Elements.Aggregate("", (a, b) => a + (a == "" ? "" : ",") + b.ToString()) + "]";
        }
    }

    public class TinymoeFunction : TinymoeObject
    {
        public Action<TinymoeObject[]> Handler { get; private set; }

        public TinymoeFunction(Action<TinymoeObject[]> value)
        {
            this.Handler = value;
        }

        public override string ToString()
        {
            return "(function)";
        }
    }

    public class TinymoeOperations
    {
        static readonly Dictionary<Tuple<Type, string>, TinymoeObject> extensions = new Dictionary<Tuple<Type, string>, TinymoeObject>();
        static TinymoeOperations()
        {
        }

        public static void Invoke(TinymoeObject function, TinymoeObject[] arguments)
        {
            ((TinymoeFunction)function).Handler(arguments);
        }

        public static void SetExtension(Type type, string name, TinymoeObject value)
        {
            extensions[Tuple.Create(type, name)] = value;
        }

        public TinymoeObject GetField(TinymoeObject target, string name)
        {
            TinymoeObject value = null;
            if (target.fields.TryGetValue(name, out value))
            {
                return value;
            }

            Type type = target.GetType();
            while (type != null)
            {
                if (extensions.TryGetValue(Tuple.Create(type, name), out value))
                {
                    return value;
                }
                type = type.BaseType;
            }

            throw new ArgumentOutOfRangeException("name");
        }

        public static TinymoeObject ArrayLength(TinymoeObject array)
        {
            return new TinymoeInteger(((TinymoeArray)array).Elements.Length);
        }

        public static TinymoeObject ArrayGet(TinymoeObject array, TinymoeObject index)
        {
            return ((TinymoeArray)array).Elements[CastToInteger(index).Value];
        }

        public static void ArraySet(TinymoeObject array, TinymoeObject index, TinymoeObject value)
        {
            ((TinymoeArray)array).Elements[CastToInteger(index).Value] = value;
        }

        private static readonly Dictionary<string, TinymoeObject> externalFunctions = new Dictionary<string, TinymoeObject>();

        public static TinymoeObject BuildExternalFunction(Func<TinymoeObject[], TinymoeObject> function)
        {
            return new TinymoeFunction(__args__ =>
            {
                var result = function(__args__.Skip(1).Take(__args__.Length - 2).ToArray());
                Invoke(__args__[2], new TinymoeObject[] { __args__[0], result });
            });
        }

        public static TinymoeObject GetExternalFunction(string name)
        {
            TinymoeObject function;
            if (!externalFunctions.TryGetValue(name, out function))
            {
                switch (name)
                {
                    case "to_n":
                        function = BuildExternalFunction(__args__ => CastToNumber(__args__[0]));
                        break;
                    case "to_i":
                        function = BuildExternalFunction(__args__ => CastToInteger(__args__[0]));
                        break;
                    case "to_f":
                        function = BuildExternalFunction(__args__ => CastToFloat(__args__[0]));
                        break;
                    case "to_s":
                        function = BuildExternalFunction(__args__ => CastToString(__args__[0]));
                        break;
                    case "Print":
                        function = BuildExternalFunction(__args__ => Print(__args__[0]));
                        break;
                    case "Sqrt":
                        function = BuildExternalFunction(__args__ => Sqrt(__args__[0]));
                        break;
                    default:
                        throw new IndexOutOfRangeException(string.Format("External function \"{0}\" does not exist.", name));
                }
                externalFunctions.Add(name, function);
            }
            return function;
        }

        public static TinymoeBoolean CastToBoolean(TinymoeObject a)
        {
            return (TinymoeBoolean)a;
        }

        public static TinymoeObject CastToNumber(TinymoeObject a)
        {
            if (a is TinymoeInteger || a is TinymoeFloat)
            {
                return a;
            }
            else
            {
                var value = (TinymoeString)a;
                int i;
                if (int.TryParse(value.Value, out i))
                {
                    return new TinymoeInteger(i);
                }
                else
                {
                    return new TinymoeFloat(double.Parse(value.Value));
                }
            }
        }

        public static TinymoeInteger CastToInteger(TinymoeObject a)
        {
            if (a is TinymoeInteger)
            {
                return (TinymoeInteger)a;
            }
            else if (a is TinymoeFloat)
            {
                return new TinymoeInteger((int)((TinymoeFloat)a).Value);
            }
            else
            {
                return new TinymoeInteger(int.Parse(((TinymoeString)a).Value));
            }
        }

        public static TinymoeFloat CastToFloat(TinymoeObject a)
        {
            if (a is TinymoeInteger)
            {
                return new TinymoeFloat((double)((TinymoeInteger)a).Value);
            }
            else if (a is TinymoeFloat)
            {
                return (TinymoeFloat)a;
            }
            else
            {
                return new TinymoeFloat(double.Parse(((TinymoeString)a).Value));
            }
        }

        public static TinymoeString CastToString(TinymoeObject a)
        {
            if (a is TinymoeInteger)
            {
                return new TinymoeString(((TinymoeInteger)a).Value.ToString());
            }
            else if (a is TinymoeFloat)
            {
                return new TinymoeString(((TinymoeFloat)a).Value.ToString());
            }
            else
            {
                return (TinymoeString)a;
            }
        }

        public static TinymoeObject Print(TinymoeObject a)
        {
            var value = CastToString(a);
            Console.WriteLine(value.Value);
            return null;
        }

        public static TinymoeObject Sqrt(TinymoeObject a)
        {
            var value = CastToFloat(a);
            return new TinymoeFloat(Math.Sqrt(value.Value));
        }
    }
}
